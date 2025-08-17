# Library-System-using-C
A procedural C program that manages a tiny library catalog using text files and a simple Windows console UI (boxes, colors, arrow‑key menus).

Note: This project is not OOP; it’s intentionally written in C with plain functions and structs.

**✨ Features**

Login screens for Student and Admin (numeric password input masked with *).

Student actions: Borrow book, Search, Return, Logout.

Admin actions: Add book, Search, Delete, Logout.

File‑based storage (CSV) for books; simple text files for users.

Console UI: bordered menus, arrow‑key navigation, colorized output.

**Main Menu**

Login as Student

Login as Admin

**Student Menu**

Borrow Book → enter ISBN; decrements copies if available.

Search for a Book → case‑insensitive title/author substring match.

Return a Book → enter ISBN; increments copies.

Logout

**Admin Menu**

Add Book → prompts for all fields; appends to file.

Search for a Book → same search as student.

Delete Book → enter ISBN; rewrites file without that record.

Logout
