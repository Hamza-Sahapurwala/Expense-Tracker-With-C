# Expense Tracker in C

A command-line **Expense Management System** written entirely in C. It supports multiple user accounts, each with their own private expense records, and can visualise spending by category as a bar graph using Gnuplot.

All data is stored in plain pipe-delimited (`|`) CSV-style text files, so there is no database or external library to set up.

---

## Tech Stack

- **Language:** C
- **Standard libraries:** `stdio.h`, `string.h`, `stdlib.h`, `ctype.h`
- **Storage:** Flat text files
- **Charts:** Gnuplot (optional)

---

## Features

- **User accounts**: sign up and log in with a username and password
- **Per-user data**: every user gets their own records file under `UserData/`
- **Add records**: amount, date, and category
- **View records**: formatted table with a total record count
- **Edit records**: change the amount, date, or category of an existing entry
- **Delete records**: remove an entry by ID (remaining records are re-indexed)
- **Bar graph**: spending totals per category, plotted with Gnuplot
- **Cross-platform screen handling**: uses `cls` on Windows and `clear` on Linux/macOS
- **Input validation**: handles non-numeric input and invalid menu choices without crashing

### Expense categories

1. Food
2. Transport
3. Clothing
4. Miscellaneous

---

## Project Structure

```
Expense-Tracker-With-C/
├── main.c        # Complete source code
├── main.exe      # Pre-built Windows executable
├── users.csv     # User credentials (username|password)
└── UserData/     # Created automatically; one <username>.csv per user
```

---

## Getting Started

### Prerequisites

- A C compiler such as **GCC** (MinGW on Windows) or Clang
- **Gnuplot** (optional, only needed for the bar graph feature)

### Build

```bash
gcc main.c -o main
```

### Run

```bash
# Windows
main.exe

# Linux / macOS
./main
```

> **Note:** Run the program from the project's root folder so it can find `users.csv` and create the `UserData/` directory in the right place.

---

## Usage

### Main menu

```
1. Login
2. Sign-Up
3. Exit
```

### User menu (after login)

```
1. Add Record
2. Delete Record
3. Edit Record
4. See Records
5. See Bar Graph of Expenses
6. Logout
7. Exit Application
```

### Sign-up rules

- Username and password must not be empty
- Neither may contain spaces or the `|` character
- Password must be at least 4 characters long
- Usernames must be unique

### Example session

```
========================================
 USER: alice - MENU
========================================

What would you like to do?
1. Add Record
...
Enter 1-7: 1

Enter Amount: 12.50
Enter Date (e.g., YYYY-MM-DD): 2025-06-01
Enter Your Category:
1. Food
...
Record added successfully with ID: 1!
```

Viewing records produces a table like:

```
ID    | Amount     | Date         | Category
------|------------|--------------|-----------------
1     | 12.50      | 2025-06-01   | Food
```

---

## Data Format

**`users.csv`**: one account per line:

```
username|password
```

**`UserData/<username>.csv`**: one expense per line:

```
ID|Amount|Date|Category
1|12.50|2025-06-01|Food
```

### Bar graph

When you choose "See Bar Graph", the program:

1. Totals the amounts for each category
2. Writes the totals to `graphdata.dat`
3. Sends a histogram plot to Gnuplot (`gnuplot -persistent`)

If Gnuplot is not installed or not on your `PATH`, the totals are still printed in the terminal and saved in `graphdata.dat`, so you can plot them manually later.

---

## Known Limitations

This project is a learning exercise, so a few things are intentionally simple:

- **Passwords are stored in plain text** in `users.csv`. Do not reuse real passwords.
- **Windows-style data path**: `USER_DATA_PATH` is defined as `.\UserData\`. On Linux/macOS you may need to change it to `./UserData/`.
- **Record limit**: edit and delete operations load at most **200 records** per user.
- **No date validation**: dates are stored as entered; the `YYYY-MM-DD` format is a convention, not enforced.
- **Re-indexing on delete**: record IDs are renumbered after a deletion, so IDs are not permanent.
- **Usernames in file paths**: the username is used directly as a filename, so avoid unusual characters.

---

## Ideas for Future Improvements

- Hash passwords instead of storing them in plain text
- Validate dates and amounts (e.g. no negative values)
- Filter and search records by date range or category
- Monthly and yearly summaries
- Export reports
- Make the data path portable across operating systems
- Remove the fixed 200-record limit with dynamic memory allocation

---