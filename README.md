## ♟️ Chess Game Engine (C++, OOPS, DSA)

A fully functional console-based chess game built from scratch in C++. Implements all standard chess rules including movement validation, captures, check, checkmate, stalemate, and castling, using clean object-oriented programming principles.

---

### 🚀 Features

✅ **Object-Oriented Design**

* Each piece (Pawn, Knight, Bishop, Rook, Queen, King) is implemented as a polymorphic class.
* Encapsulation of movement logic within each piece class.

✅ **Full Rule Enforcement**

* Piece-specific legal moves.
* Path checking for sliding pieces (rook, bishop, queen).
* Check and checkmate detection.
* Stalemate detection.
* Castling logic.
* Piece capture and updating board state.
* Prevention of illegal moves leaving king in check.

✅ **Custom Input System**

* Players move pieces by specifying:

  * Piece code (e.g. `WP1` for White Pawn 1).
  * Direction (e.g. `up`, `slantleftup`, `right`).
  * Number of steps (for sliding pieces).
* Displays full chess board after every move.

✅ **Console Visualization**

* Prints the chess board with piece codes and grid coordinates.
* Messages for captures, checks, checkmate, stalemate, and rule violations.

✅ **Memory Management**

* Dynamic allocation and cleanup of pieces.

---

### 🖥️ Technologies

* **C++** (OOP, STL: vectors, maps, strings)
* Console I/O
* Algorithm design for chess rules

---

### 🔧 How to Run

1. **Compile the code** (e.g. using g++)

```bash
g++ chess_game.cpp -o chess_game
```

2. **Run the executable**

```bash
./chess_game
```

3. **Play!**

* Enter piece codes, directions, and steps as prompted.
* Type `quit` as the piece code to exit the game.

---

### 🎮 Example Moves

* Move a white pawn up 2 steps:

  ```
  Piece code: WP2
  Direction: up
  Steps: 2
  ```

* Move a knight in an L-shape:

  ```
  Piece code: WN1
  Direction: upright
  ```

* Castle king side:

  ```
  Piece code: WKG
  Direction: castle-right
  ```

---

### ⚠️ Limitations

* No support for:

  * En passant
  * Pawn promotion to other pieces
* No graphical interface (console-only).
* No save/load game state feature.

---

### 📚 Future Improvements

* Implement en passant and pawn promotion.
* Add undo/redo functionality.
* Develop a graphical UI (using SFML, Qt, etc.).
* Add AI opponent for single-player mode.

---

### 📝 License

[MIT License](LICENSE)

---

### 🤝 Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.
