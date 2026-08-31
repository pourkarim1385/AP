# Game Matchmaking and Moderation System

A C++ console-based turn-based combat, matchmaking, and player moderation platform built using a layered architecture. The system supports role-based access control (Players and Administrators), match queuing, turn-based combat mechanics, ranking systems, and moderation tools.

---

## Architecture Overview

![StructuralDiagram](https://github.com/pourkarim1385/AP/blob/main/Structural%20Diagram.png)
![SequenceDiagram1](https://github.com/pourkarim1385/AP/blob/main/Sequence%20Diagram%201%20Player%20Action%20Execution%20(POST%20action).png)
![SequenceDiagram2](https://github.com/pourkarim1385/AP/blob/main/Sequence%20Diagram%202%20Matchmaking%20%26%20Invitation%20Acceptance%20Lifecycle.png)

The codebase is organized into distinct layers to separate domain modeling, data persistence, business logic, and user interaction:

```
AP/
├── CMakeLists.txt              # CMake build configuration
├── makefile                    # GNU Make build script
├── main.cpp                    # Application entry point and startup routines
├── DAL/                        # Data Access Layer
│   ├── Repository.h            # In-memory storage interfaces
│   └── Repository.cpp          # In-memory entity management implementation
├── Logic/                      # Presentation / Controller Layer
│   ├── RunProgram.h            # Main CLI loop and command routing declarations
│   └── RunProgram.cpp          # Command validation, permission checks, and dispatching
├── models/                     # Core Domain Entities & Utilities
│   ├── CsvFile.h / .cpp        # CSV parsing and persistence utilities
│   ├── Invitation.h / .cpp     # Game invitation data models
│   ├── Match.h / .cpp          # Turn-based match state, combat rules, and history
│   ├── Penalty.h / .cpp        # Moderation penalties (bullet and health deductions)
│   ├── Report.h / .cpp         # Player infraction reports
│   ├── ServiceException.h / .cpp # Custom application exception handling
│   ├── Types.h                 # Enums, structs, and stream formatting helpers
│   └── User.h / .cpp           # User, Player, and Admin account models
└── Service/                    # Business Logic Layer
    ├── CommandParser.h / .cpp  # Raw command line tokenization and parsing
    ├── InviteService.h / .cpp  # Invitation dispatch and response logic
    ├── LoginService.h / .cpp   # Authentication and user registration
    ├── MatchMakingService.h / .cpp # Match queueing and opponent filtering
    ├── ReportService.h / .cpp  # Incident reporting and penalty enforcement
    ├── RepositoryService.h / .cpp  # Bulk data importing from CSV files
    └── MatchServices/          # In-game state and combat services
        ├── CasualMatchService.h / .cpp # Casual combat rules and XP calculations
        ├── MatchPlayService.h / .cpp   # Match action dispatcher and status queries
        └── MatchService.h / .cpp       # Base match service interface

```

---

## Key Features

### 1. Account & Session Management

* Supports dual privilege levels: `normal` (Players) and `adminAccess` (Administrators).


* Authentication operations include user registration, login verification, and session logout.


* Player profiles track overall wins, losses, Experience Points (XP), Ranked Points (RP), and tier rankings.



### 2. Matchmaking & Social System

* **Lobby Status**: Players can toggle their casual ready status (`ready` / `unready`).


* **Opponent Discovery**: Filter opponents by match type (`casual` or `ranked`). Ranked opponents are restricted to players sharing the exact rank tier. Results can be sorted in ascending or descending order by points, with case-insensitive alphabetical tie-breaking.


* **Invitations**: Send, accept, or reject 1-on-1 match invitations. Accepting an invitation starts the match session.


* **User Blocking**: Players can block or unblock target usernames.



### 3. Turn-Based Combat Engine

* Matches run in turns where players simultaneously submit actions (`shoot`, `defend`, `reload`).


* Combat mechanics resolve shot damage, ammunition counts, and defense states.


* Match types:
* **Casual Match**: Players start with 1 bullet and 1 life. Winning adjusts player XP based on relative score differences.


* **Ranked Match**: Players start with base stats (3 bullets, 3 lives) minus any active penalty deductions. Outcomes alter Ranked Points (RP) and rank tiers.




* Match status tracking records complete move histories and remaining resources for both players.



### 4. Ranking System

Player tiers scale based on current RP values:

* **Bronze**: $\text{RP} < 1400$

* **Silver**: $1400 \le \text{RP} < 1750$

* **Gold**: $1750 \le \text{RP} < 2250$

* **Platinum**: $\text{RP} \ge 2250$


### 5. Moderation System

* Players can report other users with a specified reason.


* Administrators can view pending incident reports, dismiss reports, or issue penalties.


* Penalties apply to ranked matches over a set number of games:


* **Bullet Penalty**: Reduces starting bullets by 1 to 3.


* **Health Penalty**: Reduces starting lives by 1 to 2.





---

## Command Reference

The system accepts commands via standard input structured as:
`BASE_COMMAND target_command ? [argument_name="argument_value"]...`

### Player Commands

| Command | Method | Required Arguments | Description |
| --- | --- | --- | --- |
| `register` | `POST` | `username`, `password` | Register a new player account.

 |
| `login` | `POST` | `username`, `password` | Log into an existing account.

 |
| `logout` | `POST` | *None* | Log out of the current session.

 |
| `casual_match_ready` | `POST` | `status` (`true`/`false`) | Toggle matchmaking readiness.

 |
| `casual_match_opponents` | `GET` | `sort_order` (`asc`/`desc`, optional) | List available casual opponents.

 |
| `ranked_match_opponents` | `GET` | `sort_order` (`asc`/`desc`, optional) | List available ranked opponents in same tier.

 |
| `invitation` | `POST` | `username`, `match_type` (`casual`/`ranked`) | Send a match invitation.

 |
| `received_invitations` | `GET` | *None* | View received match invitations.

 |
| `start_match` | `POST` | `invitation_id` | Accept an invitation and start a match.

 |
| `reject_invitation` | `POST` | `invitation_id` | Reject a received match invitation.

 |
| `action` | `POST` | `action` (`shoot`/`defend`/`reload`) | Submit turn action in an active match.

 |
| `match_status` | `GET` | *None* | View active match status and move history.

 |
| `profile` | `GET` | `username` (optional) | View own or another player's profile stats.

 |
| `report` | `POST` | `username`, `reason` | Report a player for misconduct.

 |
| `block` | `POST` | `username`, `status` (`blocked`/`unblocked`) | Block or unblock a user.

 |

### Administrator Commands

| Command | Method | Required Arguments | Description |
| --- | --- | --- | --- |
| `reports` | `GET` | *None* | List all pending user reports.

 |
| `dismiss_report` | `POST` | `report_id` | Dismiss a report without penalty.

 |
| `penalty` | `POST` | `report_id`, `type` (`bullet_penalty`/`health_penalty`), `amount`, `number_of_matches` | Issue a gameplay penalty to a reported player.

 |

---

## Build & Execution

### Prerequisites

* A C++17 (or newer) compatible compiler (`g++`, `clang++`, or MSVC)
* CMake 3.10+ or GNU Make



### Compilation

Using `make`:

```bash
make

```

Using `g++` directly:

```bash
g++ -std=c++17 main.cpp DAL/*.cpp Logic/*.cpp models/*.cpp Service/*.cpp Service/MatchServices/*.cpp -o game_app

```

### Running the Application

The executable requires paths to the initial users and administrators CSV files as command-line arguments:

```bash
./game_app <path_to_users.csv> <path_to_admins.csv>

```

#### Example CSV Format

`users.csv`:

```csv
username,password,xp
player1,pass123,600
player2,pass456,800

```

`admins.csv`:

```csv
username,password
admin1,adminPass

```