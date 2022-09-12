#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int calculate_strength(pair _pair)
{
    int winner = _pair.winner;
    int loser = _pair.loser;

    return preferences[winner][loser] - preferences[loser][winner];
}

void swap(pair *xp, pair *yp)
{

    pair temp = *xp;
    *xp = *yp;
    *yp = temp;
}

bool is_cycle(int a, int b, int root);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // For each rank, check if the candidate exists
        if (strcmp(name, candidates[i]) == 0)
        {
            // I it exists, update the ranks array
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // preferences[i][j] means number of voters where candidate i (winner) is preferred over candidate j (loser)
    // for each rank, the candidate is preferred over all other candidates
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j > i)
            {
                int winner = ranks[i];
                int loser = ranks[j];
                preferences[winner][loser]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j <= i || preferences[i][j] == preferences[j][i])
            {
                continue;
            }

            // If I is the winner:
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }

            // if J is the winner:
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count - 1; j++)
        {
            // Calculate the strength of each pair
            int strengthA = calculate_strength(pairs[j]);
            int strengthB = calculate_strength(pairs[j + 1]);

            // bubble sort pairs
            if (strengthB > strengthA)
            {
                swap(&pairs[j], &pairs[j + 1]);
            }

        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    // For each pair, check if it creates a cycle.
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // If it doesn't, lock the pair.
        if (!is_cycle(winner, loser, winner))
        {
            locked[winner][loser] = true;
        }
    }
    return;
}

// Recursive function for checking if locking a pair creates a cycle
// It takes as arguments the two edges of the arrow, and a root candidate with which an arrow would create a cycle
bool is_cycle(int a, int b, int root)
{
    // If the end of the arrow is the same as the root, it would create an arrow, so return true
    if (b == root)
    {
        return true;
    }

    // check if the pair of the end of the arrow creates a cycle
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[b][i] == true && is_cycle(b, i, root))
        {
            return true;
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // For each candidate, check if there is any arrow pointing toit.
    for (int i = 0; i < candidate_count; i++)
    {
        bool isWinner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                isWinner = false;
            }
        }

        // If not, this candidate is the winner 
        if (isWinner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}