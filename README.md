# Rust Learning Projects

This repository is a collection of small Rust projects intended for an experienced
C++ and C# developer. The projects progress from ownership and borrowing through
concurrency, async programming, networking, and FFI.

Each specification describes required behavior from a user's perspective. The
implementation, internal data structures, dependencies, and command-line parsing
libraries are intentionally left open unless they are part of the learning goal.

## Table of Contents

- [Suggested Order](#suggested-order)
- [Project 1: Local Key/Value Cache](#project-1-local-keyvalue-cache)
- [Project 2: CLI Log Analyzer](#project-2-cli-log-analyzer)
- [Project 3: Mini Grep](#project-3-mini-grep)
- [Project 4: Duplicate-File Finder](#project-4-duplicate-file-finder)
- [Project 5: Multithreaded File Indexer](#project-5-multithreaded-file-indexer)
- [Project 6: Arena-Based Object Graph](#project-6-arena-based-object-graph)
- [Project 7: Thread-Pool Implementation](#project-7-thread-pool-implementation)
- [Project 8: TCP Chat Server](#project-8-tcp-chat-server)
- [Project 9: Async HTTP Service](#project-9-async-http-service)
- [Project 10: C or C++ Library Wrapper](#project-10-c-or-c-library-wrapper)
- [Rust and Cargo Command Reference](#rust-and-cargo-command-reference)
  - [Toolchain Setup and Updates](#toolchain-setup-and-updates)
  - [Creating Projects](#creating-projects)
  - [Fast Development Loop](#fast-development-loop)
  - [Building and Running](#building-and-running)
  - [Testing](#testing)
  - [Formatting and Linting](#formatting-and-linting)
  - [Dependencies](#dependencies)
  - [Documentation](#documentation)
  - [Package and Workspace Selection](#package-and-workspace-selection)
  - [Feature Flags](#feature-flags)
  - [Benchmarks](#benchmarks)
  - [Inspecting the Project](#inspecting-the-project)
  - [Environment and Backtraces](#environment-and-backtraces)
  - [Publishing Checks](#publishing-checks)
  - [Useful Non-Cargo Commands](#useful-non-cargo-commands)
- [Definition of Done for Each Project](#definition-of-done-for-each-project)

## Suggested Order

1. Local key/value cache
2. CLI log analyzer
3. Mini grep
4. Duplicate-file finder
5. Multithreaded file indexer
6. Arena-based object graph
7. Thread-pool implementation
8. TCP chat server
9. Async HTTP service
10. C or C++ library wrapper

For each project:

- Include unit tests for core domain behavior.
- Include integration tests for important user-facing workflows where practical.
- Return useful errors instead of panicking on expected bad input.
- Run `cargo fmt`, `cargo clippy`, and `cargo test` before considering it complete.
- Do not use `clone()` solely to bypass an ownership or borrowing problem.

---

## Project 1: Local Key/Value Cache

### Goal

Build a local command-line cache that stores string keys and values. Begin with
an in-memory cache, then add expiration, eviction, and persistence.

### User Interface

The program starts an interactive prompt and supports these commands:

```text
set <key> <value>
get <key>
remove <key>
list
clear
exit
```

Later milestones add:

```text
set <key> <value> --ttl <seconds>
stats
save
```

### Acceptance Criteria: Basic Cache

- Given an empty cache, when the user sets a key and gets it, the stored value is
  displayed.
- Setting an existing key replaces its value and displays that it was updated.
- Getting a missing key displays a clear "not found" result.
- Removing an existing key returns or displays the removed value.
- Removing a missing key does not fail or terminate the program.
- `list` displays every current key/value pair.
- `list` on an empty cache displays an explicit empty-cache message.
- `clear` removes all entries.
- Keys and values containing valid Unicode text are supported.
- Malformed and unknown commands display usage help without terminating the
  interactive session.
- `exit` ends the program cleanly.

### Acceptance Criteria: Expiration

- A user can assign a TTL in whole seconds when setting a value.
- A value is available before its TTL expires.
- An expired value behaves as if it does not exist.
- Expired entries do not appear in `list`.
- Replacing a key replaces its previous expiration time.
- A value set without a TTL remains available until removed, evicted, or cleared.
- Zero and invalid TTL values produce a useful validation error.

### Acceptance Criteria: Capacity and LRU Eviction

- The user can configure a positive maximum number of entries at startup.
- Inserting within the configured capacity does not evict entries.
- Inserting beyond capacity evicts the least recently used entry.
- Successfully getting an entry marks it as recently used.
- Updating an entry marks it as recently used.
- Expired entries are preferred for removal before a live entry is evicted.
- `stats` reports at least entry count, capacity, cache hits, cache misses, and
  evictions.

### Acceptance Criteria: Persistence

- The user can configure a persistence file.
- `save` writes all non-expired entries to that file.
- A clean program exit saves the cache automatically.
- Starting the program with an existing persistence file restores valid entries.
- Expired persisted entries are not restored.
- A missing persistence file starts an empty cache.
- A malformed or unreadable persistence file produces a clear error and never
  silently discards or overwrites its contents.
- Saving is performed safely enough that an interrupted save does not replace a
  previously valid file with a partially written file.

### Learning Focus

Ownership, borrowing, `String` versus `&str`, `HashMap`, `Option`, `Result`,
enums, iterators, time handling, serialization, and testing.

---

## Project 2: CLI Log Analyzer

### Goal

Build a command-line application that reads one or more log files and summarizes
their contents.

Assume the first supported format is:

```text
2026-09-23T14:32:11Z ERROR database connection failed
2026-09-23T14:32:12Z INFO request completed
```

### Example Usage

```powershell
cargo run -- analyze .\samples\app.log
cargo run -- analyze .\logs\*.log --level ERROR
cargo run -- analyze .\samples\app.log --contains "database"
cargo run -- analyze .\samples\app.log --from 2026-09-23T14:00:00Z
```

### Acceptance Criteria

- The user can analyze one file, multiple files, or a file pattern.
- The summary reports total valid lines and counts grouped by log level.
- Supported levels include at least `TRACE`, `DEBUG`, `INFO`, `WARN`, and
  `ERROR`.
- The user can filter results by one or more log levels.
- The user can filter messages by a case-sensitive or case-insensitive substring.
- The user can filter entries using inclusive `--from` and `--to` timestamps.
- Multiple filters combine predictably and are documented.
- Results can be displayed as a human-readable table.
- An optional machine-readable JSON output mode is available.
- Empty files and files with no matching entries produce a successful, explicit
  "no matching entries" result.
- Malformed lines are counted and skipped rather than crashing the application.
- The final output reports the number of malformed lines.
- An unreadable or missing input file identifies the affected path and returns a
  nonzero exit code.
- Large files are processed without loading the entire file into memory.
- Files are processed in a deterministic order so repeated runs produce stable
  output.

### Stretch Criteria

- Report the most frequent messages.
- Group messages into time buckets.
- Support JSON Lines logs.
- Follow a growing file similarly to `tail -f`.

### Learning Focus

Buffered I/O, iterators, parsing, enums, pattern matching, error propagation,
date/time handling, sorting, and command-line interfaces.

---

## Project 3: Mini Grep

### Goal

Build a smaller version of `grep` that searches text files and prints matching
lines.

### Example Usage

```powershell
cargo run -- "timeout" .\logs\app.log
cargo run -- "error" .\src --recursive --ignore-case
cargo run -- "TODO" . --recursive --line-number --extension rs
```

### Acceptance Criteria

- The user can search for a literal string in a single file.
- Matching lines are written to standard output.
- No matches produce no result lines and a documented nonzero exit code distinct
  from an operational error.
- `--line-number` prefixes matches with one-based line numbers.
- `--ignore-case` performs case-insensitive matching.
- `--invert-match` returns lines that do not contain the search text.
- `--count` reports the number of matching lines instead of printing them.
- The user can search a directory recursively.
- Directory searches display the file path with each match.
- The user can filter recursive searches by file extension.
- Binary files are skipped with a clear, documented policy.
- Unreadable files are reported to standard error without hiding matches from
  other readable files.
- Input can be read from standard input when no file is supplied or when `-` is
  used as the path.
- Output ordering is deterministic.
- Search processing is streaming and does not require loading entire files.

### Stretch Criteria

- Add regular-expression support.
- Add configurable context lines before and after each match.
- Add colored highlighting when writing to an interactive terminal.

### Learning Focus

String slices, lifetimes, iterators, buffered I/O, paths, standard input/output,
exit codes, and reusable library-versus-binary design.

---

## Project 4: Duplicate-File Finder

### Goal

Build a command-line tool that finds files with identical content beneath one or
more directories.

### Example Usage

```powershell
cargo run -- scan C:\Photos C:\Backups
cargo run -- scan . --minimum-size 1MB
cargo run -- scan . --exclude target --format json
```

### Acceptance Criteria

- The user can scan one or more directory roots.
- Duplicate groups contain only files whose complete contents are identical.
- Files with equal names or equal sizes but different contents are never reported
  as duplicates.
- Each duplicate group displays file size, wasted space, and every matching path.
- The summary displays the number of files scanned, duplicate groups found, and
  total potentially recoverable space.
- Results are ordered deterministically.
- The user can specify a minimum file size.
- The user can exclude directories or path patterns.
- Symbolic links and directory junctions have a documented policy and do not
  cause traversal loops.
- Permission errors identify affected paths while allowing the remainder of the
  scan to continue.
- Files that change during scanning are detected or reported rather than silently
  producing unreliable results.
- An optional JSON output format is provided.
- The application does not modify or delete any files.
- The implementation avoids hashing every byte of obviously unique files, such
  as files with unique sizes.

### Stretch Criteria

- Add an interactive deletion mode with confirmation and a dry-run option.
- Cache file metadata and hashes between runs.
- Hash candidate files concurrently.

### Learning Focus

`Path` and `PathBuf`, filesystem metadata, hashing, maps, grouping, robust error
handling, and safe concurrency.

---

## Project 5: Multithreaded File Indexer

### Goal

Build an application that indexes files under a directory and lets users query
file names, paths, and optionally text contents.

### Example Usage

```powershell
cargo run -- index C:\code --database .\index.json
cargo run -- search .\index.json "Cache"
cargo run -- search .\index.json "connection timeout" --content
```

### Acceptance Criteria: Indexing

- The user can create an index for a selected directory.
- The index records at least each file's path, size, and last-modified time.
- Optional content indexing supports documented text file types.
- The user can choose the number of worker threads.
- A single-thread configuration produces the same logical index as a
  multithreaded configuration.
- The program displays progress for a nontrivial scan.
- Permission failures and unreadable files are reported without aborting the
  complete index operation.
- Symbolic links and junctions follow a documented policy and do not create loops.
- The completed index is saved to a user-selected file.
- A failed or interrupted index operation does not replace a previously valid
  index with a partial one.

### Acceptance Criteria: Searching

- The user can search indexed file names and paths.
- Content search is available when content indexing was enabled.
- Results include enough path information to locate each file.
- Search supports case-sensitive and case-insensitive modes.
- The user can limit the maximum number of results.
- Results are deterministic for identical index data and query options.
- Opening a malformed or unsupported index produces a clear error.

### Acceptance Criteria: Incremental Updates

- Re-indexing can identify new, modified, and deleted files.
- Unchanged files are not reread unnecessarily.
- The summary reports counts for added, updated, unchanged, deleted, and failed
  files.

### Learning Focus

Threads, channels, worker coordination, `Arc`, `Mutex` or `RwLock`, `Send`,
`Sync`, cancellation, paths, persistence, and deterministic concurrent systems.

---

## Project 6: Arena-Based Object Graph

### Goal

Build a graph-oriented application in which objects are stored centrally and
refer to each other using stable IDs rather than pointers or references.

A dependency graph, family tree, or transit map are all suitable user-facing
domains.

### Example Commands

```text
add-node compiler
add-node parser
add-edge compiler parser
neighbors compiler
path compiler parser
remove-node parser
```

### Acceptance Criteria

- The user can add a node and receives or can see a stable node ID.
- Nodes can have human-readable names and optional metadata.
- The user can connect two existing nodes with an edge.
- Attempts to connect nonexistent nodes return a clear error.
- The user can list a node's incoming and outgoing neighbors.
- The user can find a path between two nodes when one exists.
- A missing path produces an explicit "no path" result.
- Removing an edge leaves both nodes intact.
- Removing a node does not leave dangling usable edges.
- Removing one node does not accidentally change another live node's identity.
- Duplicate node names have an explicit policy: either rejected or supported
  through unique IDs.
- The graph can be saved and loaded while preserving node identities and edges.
- Loading malformed graph data produces a clear error.
- Core graph operations are exposed through a reusable library API and exercised
  through tests.

### Stretch Criteria

- Detect cycles.
- Produce a topological ordering for acyclic graphs.
- Reuse removed storage slots while preventing stale IDs from referring to newly
  inserted nodes, for example with generational IDs.

### Learning Focus

Ownership-friendly data modeling, newtype IDs, indexes, enums, graph traversal,
serialization, and alternatives to pointer-heavy object graphs.

---

## Project 7: Thread-Pool Implementation

### Goal

Implement a reusable fixed-size thread pool and demonstrate it with a small
batch-processing application.

### Example User Workflow

```powershell
cargo run -- process .\inputs --threads 4
```

The demo application can hash files, resize test images, or perform simulated
jobs.

### Acceptance Criteria: Library

- A caller can create a pool with a positive worker count.
- Creating a pool with zero workers returns a clear error or is rejected by the
  API contract.
- A caller can submit independent jobs for execution.
- Every accepted job runs at most once.
- Multiple jobs can execute concurrently when multiple workers are configured.
- A panic in one job is reported or isolated according to a documented policy
  and does not silently kill the entire pool.
- The caller can request a graceful shutdown.
- Graceful shutdown stops accepting work and waits for all accepted jobs.
- Dropping the pool cannot leave worker threads unintentionally running.
- The API provides a way to wait until all submitted jobs are complete.
- The API has a documented policy for jobs submitted during or after shutdown.

### Acceptance Criteria: Demo Application

- The user can select the worker count.
- The user receives progress and a final success/failure summary.
- A failure in one input does not hide results for other inputs.
- Running with one worker produces the same logical output as running with
  multiple workers.
- Invalid arguments return a useful error and nonzero exit code.

### Stretch Criteria

- Return typed job results through handles or channels.
- Support bounded queues and backpressure.
- Support cancellation.
- Compare fixed-size and work-stealing designs.

### Learning Focus

Threads, closures, trait bounds, channels, synchronization, RAII cleanup,
panic boundaries, `Drop`, `Send`, and `'static`.

---

## Project 8: TCP Chat Server

### Goal

Build a server that accepts multiple TCP clients and lets them exchange messages.
A simple terminal client should also be provided.

### Example Usage

```powershell
cargo run --bin chat-server -- --address 127.0.0.1:7000
cargo run --bin chat-client -- --address 127.0.0.1:7000 --name alice
```

### Acceptance Criteria: Server

- The user can start the server on a configurable address and port.
- Multiple clients can connect simultaneously.
- A connecting client chooses a nonempty display name.
- Duplicate active display names are rejected with a useful message.
- A message sent by one client is delivered to every other connected client.
- Join and leave notifications are broadcast to connected clients.
- A disconnected or misbehaving client does not terminate the server.
- Client input has a documented maximum message size.
- Oversized or invalid messages are rejected without unbounded memory growth.
- The server removes disconnected clients from active state.
- The server handles shutdown cleanly and closes client connections.
- Network and protocol failures are logged with useful context.

### Acceptance Criteria: Client

- The user can configure server address and display name.
- The client displays messages while still accepting user input.
- `/quit` disconnects cleanly.
- Failure to connect displays a clear error and returns a nonzero exit code.
- Server disconnection is reported rather than appearing as a frozen client.

### Stretch Criteria

- Add private messages and chat rooms.
- Add message timestamps.
- Add a framed JSON protocol.
- Implement both a thread-per-client server and an async server, then compare
  them.

### Learning Focus

TCP, concurrent reads and writes, shared state, channels, framing, connection
lifecycle management, and graceful shutdown.

---

## Project 9: Async HTTP Service

### Goal

Build an asynchronous JSON HTTP API using Tokio, Axum, and Serde. The service can
manage notes, tasks, or cached values.

The examples below assume a task service:

```text
POST   /tasks
GET    /tasks
GET    /tasks/{id}
PUT    /tasks/{id}
DELETE /tasks/{id}
GET    /health
```

### Acceptance Criteria: API Behavior

- `POST /tasks` creates a task from valid JSON and returns `201 Created`.
- A created task has a server-generated unique ID.
- Missing or invalid required fields return `400 Bad Request` with a structured
  JSON error.
- `GET /tasks` returns a JSON array of tasks.
- `GET /tasks/{id}` returns the matching task.
- Getting a missing task returns `404 Not Found`.
- `PUT /tasks/{id}` updates the selected task and returns its current
  representation.
- Updating a missing task returns `404 Not Found`.
- `DELETE /tasks/{id}` removes an existing task and returns a documented success
  response.
- Deleting a missing task returns `404 Not Found`.
- `GET /health` reports whether the process can serve requests.
- Every response uses an appropriate content type and HTTP status.
- Internal failures return a structured error without exposing backtraces or
  sensitive implementation details.

### Acceptance Criteria: Querying and Concurrency

- `GET /tasks` supports pagination with validated limits.
- The user can filter tasks by at least one field, such as completion status.
- Concurrent requests cannot corrupt application state.
- Two tasks created concurrently receive distinct IDs.
- A slow request does not prevent unrelated requests from being served.

### Acceptance Criteria: Operations

- The listen address is configurable.
- Startup configuration errors prevent the service from claiming it started.
- Requests have structured logs containing method, path, status, and duration.
- The service handles Ctrl+C with graceful shutdown.
- Automated tests exercise routes without requiring a manually started server.

### Stretch Criteria

- Replace in-memory storage with SQLite or PostgreSQL.
- Add optimistic concurrency with version numbers or ETags.
- Add OpenAPI documentation.
- Add authentication only after the core service is complete.

### Learning Focus

Async/await, Tokio, Axum extractors, Serde, shared application state, middleware,
HTTP semantics, structured errors, integration testing, and graceful shutdown.

---

## Project 10: C or C++ Library Wrapper

### Goal

Wrap a small native C API in a safe, idiomatic Rust interface. Prefer a simple,
well-defined library or write a tiny C library specifically for the exercise.

One suitable sample library could expose:

```c
native_context* context_create(void);
int context_add(native_context* context, const char* text);
const char* context_get(native_context* context, size_t index);
void context_destroy(native_context* context);
```

### Acceptance Criteria: Raw FFI Layer

- Rust can build or link the native library using a repeatable Cargo command.
- Native functions and types are declared in a dedicated low-level module or
  `-sys` crate.
- C integer, pointer, and string types are represented correctly.
- Native error codes are preserved and translated rather than ignored.
- The unsafe boundary is small and clearly identifiable.

### Acceptance Criteria: Safe Wrapper

- A Rust user can create and use the native object without writing `unsafe`.
- Native resources are released automatically when the Rust wrapper is dropped.
- The wrapper cannot be safely used after its native resource is released.
- Null pointers and native error codes become explicit Rust errors.
- Rust strings are validated before conversion to C strings.
- Strings returned by the native library are copied or borrowed according to a
  documented lifetime contract.
- The safe API does not expose unrestricted raw pointers.
- Ownership transfer between Rust and the native library is documented and
  tested.
- Repeated create/use/drop cycles do not leak native resources.
- Calling the wrapper with invalid input produces an error rather than undefined
  behavior.

### Acceptance Criteria: Threading and Build Portability

- The wrapper explicitly documents whether values are safe to send or share
  across threads.
- `Send` or `Sync` is not implemented unless the native library's guarantees
  justify it.
- Build failures identify missing compilers, headers, or libraries clearly.
- The project documents how to build it on Windows.

### Stretch Criteria

- Expose the Rust library back to C with a C ABI.
- Generate bindings with `bindgen` and compare them with handwritten bindings.
- Use the `cc` crate to compile a bundled C source file.
- Wrap a callback-based native API and safely handle callback state.

### Learning Focus

FFI, `unsafe`, raw pointers, `CString` and `CStr`, RAII, `Drop`, error mapping,
ABI boundaries, build scripts, native linking, and thread-safety contracts.

---

## Rust and Cargo Command Reference

Run Cargo commands from the directory containing the relevant `Cargo.toml`.

### Toolchain Setup and Updates

```powershell
rustup --version                 # Show rustup version
rustup show                      # Show installed and active toolchains
rustup update                    # Update installed Rust toolchains
rustup toolchain install stable  # Install the stable toolchain
rustup default stable            # Make stable the default toolchain
rustup component add rustfmt     # Install the formatter
rustup component add clippy      # Install the Clippy linter
rustup doc                       # Open local Rust documentation
rustc --version                  # Show compiler version
cargo --version                  # Show Cargo version
```

### Creating Projects

```powershell
cargo new my-app                 # Create a binary application
cargo new my-library --lib       # Create a library
cargo init                       # Create a package in the current directory
cargo init --lib                 # Create a library in the current directory
```

Package names normally use kebab-case, while Rust crate identifiers use
underscores in source code:

```text
package: my-cache
crate:   my_cache
```

### Fast Development Loop

```powershell
cargo check                      # Type-check without producing a final binary
cargo run                        # Build and run the default binary
cargo run -- arg1 arg2           # Pass arguments to the program
cargo test                       # Run all tests
cargo fmt                        # Format the package
cargo clippy                     # Run Rust-specific lint checks
```

A useful pre-completion check is:

```powershell
cargo fmt --check
cargo clippy --all-targets --all-features -- -D warnings
cargo test --all-features
```

### Building and Running

```powershell
cargo build                      # Debug build
cargo build --release            # Optimized release build
cargo run --release              # Run an optimized build
cargo run --bin chat-server      # Run a specific binary
cargo run --example demo         # Run an example from examples\
cargo clean                      # Delete generated build artifacts
```

Debug output is normally under `target\debug`; optimized output is under
`target\release`.

### Testing

```powershell
cargo test                       # Run all tests
cargo test cache                 # Run tests whose names contain "cache"
cargo test exact_test_name -- --exact
cargo test --lib                 # Run library unit tests
cargo test --test api            # Run tests\api.rs
cargo test --doc                 # Run documentation tests
cargo test -- --nocapture        # Display output printed by passing tests
cargo test -- --test-threads=1   # Run tests serially
cargo test --release             # Test optimized code
```

Show available tests:

```powershell
cargo test -- --list
```

### Formatting and Linting

```powershell
cargo fmt                        # Format files
cargo fmt --check                # Check formatting without changing files
cargo clippy                     # Lint the current package
cargo clippy --all-targets       # Include tests, examples, and binaries
cargo clippy --fix               # Apply supported automatic fixes
cargo clippy -- -D warnings      # Treat all warnings as errors
```

Use plain `cargo fix` for compiler-suggested migrations and fixes:

```powershell
cargo fix
cargo fix --edition
```

Review automatic changes before committing them.

### Dependencies

```powershell
cargo add serde                  # Add a dependency
cargo add serde --features derive
cargo add tokio --features full
cargo add tempfile --dev         # Add a development dependency
cargo remove serde               # Remove a dependency
cargo update                     # Update dependencies within allowed versions
cargo tree                       # Display the dependency graph
cargo tree -d                    # Display duplicate dependency versions
cargo metadata --format-version 1
```

`cargo add` and `cargo remove` are available in modern Cargo versions. Dependency
requirements are stored in `Cargo.toml`; exact resolved versions are recorded in
`Cargo.lock`.

### Documentation

```powershell
cargo doc                        # Build dependency and package documentation
cargo doc --open                 # Build and open documentation
cargo doc --no-deps --open       # Document only the current package
cargo test --doc                 # Test Rust examples in documentation comments
```

Common documentation comments:

```rust
/// Documents the following item.
pub fn public_function() {}

//! Documents the containing module or crate.
```

### Package and Workspace Selection

These commands are useful after this repository has a Cargo workspace:

```powershell
cargo check --workspace          # Check every workspace member
cargo test --workspace           # Test every workspace member
cargo test -p local-cache        # Test one package
cargo run -p local-cache         # Run one package
cargo clippy --workspace --all-targets
```

### Feature Flags

```powershell
cargo check --features persistence
cargo test --all-features
cargo test --no-default-features
cargo run --features json -- input.log
```

Features should represent optional capabilities, not runtime configuration.

### Benchmarks

Stable Rust supports benchmark targets through libraries such as Criterion:

```powershell
cargo bench
cargo bench cache_get
```

Use release-mode timing and compare behavior with representative inputs rather
than relying on a single short run.

### Inspecting the Project

```powershell
cargo locate-project             # Find the active Cargo.toml
cargo pkgid                      # Display the current package identifier
cargo tree                       # Inspect dependencies
cargo metadata --format-version 1
rustc --explain E0382            # Explain a compiler error code
```

Compiler error explanations are especially useful while learning ownership:

```powershell
rustc --explain E0382            # Use of a moved value
rustc --explain E0499            # Multiple mutable borrows
rustc --explain E0502            # Mutable and immutable borrow conflict
rustc --explain E0597            # Borrowed value does not live long enough
```

### Environment and Backtraces

Set environment variables for one PowerShell session:

```powershell
$env:RUST_BACKTRACE = "1"        # Show a panic backtrace
$env:RUST_BACKTRACE = "full"     # Show a more detailed backtrace
$env:RUST_LOG = "debug"          # Common logging filter used by logging crates
cargo run
```

Remove a variable:

```powershell
Remove-Item Env:\RUST_BACKTRACE
```

### Publishing Checks

Even for packages that will not be published, these commands help verify package
metadata and included files:

```powershell
cargo package --list             # Show files that would be packaged
cargo package                    # Build a distributable crate package
cargo publish --dry-run          # Validate publishing without uploading
```

Do not run `cargo publish` unless the package is intentionally ready for
crates.io.

### Useful Non-Cargo Commands

```powershell
rustfmt .\src\main.rs            # Format an individual Rust file
rustc .\small-example.rs         # Compile a standalone Rust source file
rustc .\small-example.rs -o .\small-example.exe
```

Prefer Cargo for normal projects because it manages crate metadata,
dependencies, test targets, build profiles, and generated artifacts.

---

## Definition of Done for Each Project

A project is complete when:

- All required acceptance criteria are satisfied.
- Expected user errors produce clear messages and appropriate exit codes or HTTP
  statuses.
- Core behavior has automated tests, including important failure cases.
- The project has its own short usage documentation and examples.
- `cargo fmt --check` succeeds.
- `cargo clippy --all-targets --all-features -- -D warnings` succeeds.
- `cargo test --all-features` succeeds.
- Release mode builds successfully with `cargo build --release`.
- The implementation does not contain unexplained `unsafe`, unnecessary cloning,
  or ignored errors.
