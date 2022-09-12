import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
from math import floor

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    user_stocks = db.execute("SELECT symbol, amount FROM user_shares WHERE user_id = ?", session["user_id"])
    stock_total = 0
    for stock in user_stocks:
        share = lookup(stock['symbol'])
        stock_total += share['price'] * stock['amount']
    return render_template("index.html", user=user[0], user_stocks=user_stocks, lookup=lookup, float=float, round=round, stock_total=stock_total, floor=floor, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get('symbol')
        share = lookup(symbol)
        number = 0
        try:
            number = int(request.form.get('shares'))
        except:
            return apology("Invalid number of shares")
        if not share:
            return apology("Invalid symbol")
        elif not number or number <= 0:
            return apology("Invalid number of shares")
        else:
            price = round(float(share['price']), 2)
            user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
            user_cash = round(float(user[0]['cash']), 2)
            if price * number > user_cash:
                return apology("Can't afford")
            db.execute("INSERT INTO transactions (user_id, share, share_amount, price, date) VALUES (?, ?, ?, ?, ?)",
                session["user_id"], symbol, number, price * number, datetime.datetime.now())
            cash_left = user_cash - price * number
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_left, session["user_id"])
            symbol_shares = db.execute("SELECT * FROM user_shares WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
            if len(symbol_shares) != 1:
                db.execute("INSERT INTO user_shares (user_id, symbol, amount) VALUES (?, ?, ?)", session["user_id"], symbol, number)
            else:
                share_amount = int(symbol_shares[0]["amount"])
                share_amount += number
                db.execute("UPDATE user_shares SET amount = ? WHERE user_id = ? AND symbol = ?",
                    share_amount, session["user_id"], symbol)

            return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions")
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        share = lookup(symbol)

        if share == None:
            return apology("Invalid symbol")
        else:
            return render_template("quote.html", share=share, usd=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username or not password or not confirmation:
             return apology("Please fill all the fields")

        elif password != confirmation:
            return apology("Passwords don't match")
        elif any(d['username'] == username for d in db.execute("SELECT username FROM users")):
            return apology("Username already exists")
        else:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
            return render_template("login.html")


    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        symbol = request.form.get('symbol')
        number = int(request.form.get('shares'))
        options = []
        user_shares = db.execute("SELECT symbol FROM user_shares WHERE user_id = ?", session["user_id"])
        share_amount = int(db.execute("SELECT amount FROM user_shares WHERE symbol = ? AND user_id = ?",
            symbol, session["user_id"])[0]['amount'])
        for share in user_shares:
            options.append(share['symbol'])

        if not symbol or symbol not in options:
            return apology("Please provide a valid share")

        elif not number or number <= 0:
            return apology("Please provide a valid number of stocks")

        elif share_amount < number:
            return apology("Not enough shares to sell")
        else:
            share = lookup(symbol)
            final_amount = share_amount - number
            db.execute("UPDATE user_shares SET amount = ? WHERE symbol = ? AND user_id = ?",
                    final_amount, symbol, session["user_id"])
            current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
            final_cash = current_cash + share['price'] * number
            db.execute("UPDATE users SET cash = ? WHERE id = ?", final_cash, session["user_id"])
            db.execute("INSERT INTO transactions (user_id, share, share_amount, price, date) VALUES (?, ?, ?, ?, ?)",
                    session["user_id"], symbol, -number, share['price'] * number, datetime.datetime.now())
            if final_amount == 0:
                db.execute("DELETE FROM user_shares WHERE symbol = ? AND user_id = ?", symbol, session["user_id"])
            return redirect("/")

    else:
        user_shares = db.execute("SELECT symbol FROM user_shares WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", user_shares=user_shares)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)