-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.


SELECT transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


SELECT people.name FROM bakery_security_logs
JOIN people ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit";
-- SUSPECTS:
| Vanessa |
| Bruce   |
| Barry   |
| Luca    |
| Sofia   |
| Iman    |
| Diana   |
| Kelsey  |


SELECT people.name FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.transaction_type = 'withdraw'
AND people.name IN (
    SELECT people.name FROM bakery_security_logs
    JOIN people ON bakery_security_logs.license_plate = people.license_plate
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
);
-- SUSPECTS
| Bruce |
| Diana |
| Iman  |
| Luca  |


SELECT * FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE year = 2021 AND month = 7 AND day = 28
AND duration <= 60
AND (name = "Bruce" OR name = "Diana" OR name = "Iman" OR name = "Luca");

-- SUSPECTS
| Bruce |
| Diana |

SELECT name FROM people
WHERE phone_number IN (
    SELECT receiver FROM phone_calls
    JOIN people ON people.phone_number = phone_calls.caller
    WHERE year = 2021 AND month = 7 AND day = 28
    AND duration <= 60
    AND (name = "Bruce" OR name = "Diana" OR name = "Iman" OR name = "Luca")
);
-- ACCOMPLICES
| Philip | - Diana
| Robin  | - Bruce


SELECT people.name, airports.city FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
JOIN airports ON flights.destination_airport_id = airports.id
WHERE flights.year = 2021 AND flights.month = 7 AND flights.day = 29
AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
AND (people.name = "Bruce" OR people.name = "Diana" OR people.name = "Philip" OR people.name = "Robin" )
ORDER BY hour, minute;
| Bruce | New York City |
| Diana | Boston        |

-- Bruce is the thief, because the flight was before Diana's
-- Robin was his accomplice
-- They flew to New York City

