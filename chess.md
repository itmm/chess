# Chess
* a small chess playing program

```
@Def(file: chess.cpp)
	@put(globals)
	int main() {
		@put(setup);
		@put(main loop);
	}
@End(file: chess.cpp)
```
* simple `@f(main)` function

```
@def(globals)
	#include <iostream>
@end(globals)
```
* needs `std::cin`, `std::cout`, `std::cerr`

```
@def(main loop)
	for (;;) {
		std::cout << "?> ";
		std::string cmd;
		if (! (std::cin >> cmd)) {
			break;
		}
		@put(do cmd);
		@put(unknown cmd);
	}
@end(main loop)
```
* simple read/process loop
* terminate on end of file

```
@def(unknown cmd)
	std::cerr << "unknown command " <<
		cmd << "\n";
@end(unknown cmd)
```
* message for unknown inputs

## Print board
* first task is to print the board

```
@add(globals)
	signed char board[120] {
		@put(initial board)
	};
@end(globals)
```
* board consists of 12 rows (ranks) each row having 10 columns (files)
* the boarder simplifies checking of legal moves
* especially for legal knight moves
* the boarder is initialized with 99

```
@def(initial board)
	99,99,99,99,99,99,99,99,99,99,
	99,99,99,99,99,99,99,99,99,99,
	99, 2, 4, 3, 5, 6, 3, 4, 2,99,
	99, 1, 1, 1, 1, 1, 1, 1, 1,99,
	99, 0, 0, 0, 0, 0, 0, 0, 0,99,
	99, 0, 0, 0, 0, 0, 0, 0, 0,99,
	99, 0, 0, 0, 0, 0, 0, 0, 0,99,
	99, 0, 0, 0, 0, 0, 0, 0, 0,99,
	99,-1,-1,-1,-1,-1,-1,-1,-1,99,
	99,-2,-4,-3,-5,-6,-3,-4,-2,99,
	99,99,99,99,99,99,99,99,99,99,
	99,99,99,99,99,99,99,99,99,99
@end(initial board)
```
* empty fields are `0`
* white figures have positive values
* black figures have negative values
* the board is rotated: the top left corner is A1

```
@def(setup)
	const std::string _pieces_board[] {
		"♚", "♛", "♞", "♝", "♜",
		"♟", ".", "♙", "♖",
		"♗", "♘", "♕", "♔"
	};
	const std::string *pieces_board {
		_pieces_board + 6
	};
@end(setup)
```
* UTF-8 character codes are available for all chess figures
* a pointer to the empty field is used to allow for negative indices

```
@def(do cmd)
	if (cmd == "p") {
		std::cout << "\n";
		@put(print board);
		continue;
	}
@end(do cmd)
```
* the `p` command prints the board

```
@def(print board)
	for (int r = 7; r >= 0; --r) {
		std::cout << (r + 1) << " ";
		@put(print board rank);
		std::cout << "\n";
	}
	std::cout << "  abcdefgh\n\n";
@end(print board)
```
* go through each rank in reverse order
* print the rank
* and close with the file indices
* each rank starts with the rank index

```
@add(globals)
	inline int f_r_to_idx(
		int f, int r
	) {
		return r * 10 + f + 21;
	}
@end(globals)
```
* get index for file `f` and rank `r`
* each rank consists of 10 bytes
* and the first two ranks must be skipped
* so must the first file

```
@add(globals)
	inline signed char get(
		int f, int r
	) {
		return board[f_r_to_idx(f, r)];
	}
@end(globals)
```
* get piece value for a specific file `f` and rank `r`

```
@def(print board rank);
	for (int f = 0; f <= 7; ++f) {
		std::cout << pieces_board[
			get(f, r)
		];
	}
@end(print board rank);
```
* print figure

## Handle Material
* two counts for material are kept
* the absolute value indicates how many pieces are still in play
* the normal value indicates how big the advantage of white is

```
@add(globals)
	float material { 0.0f };
	float abs_material { 481.0f };
@end(globals)
```
* initialize values for full board
* no advantage for any color

```
@add(print board)
	std::cout << "  Material: " <<
		abs_material << " (" <<
		material << ")\n\n";
@end(print board)
```
* print absolute and relative value

```
@add(globals)
	const float _pieces_mat[] {
		-200.0f, -9.0f, -3.25f, -3.5f,
		-5.0f, -1.0f, 0.0f, 1.0f, 5.0f,
		3.5f, 3.25f, 9.0f, 200.0f
	};
	const float *pieces_mat {
		_pieces_mat + 6
	};
@end(globals)
```
* each piece has a different value
* offset is used again to allow negative indices

```
@add(globals)
	#include <cmath>
@end(globals)
```
* needs `@f(fabs)

```
@add(globals)
	void add(signed char f) {
		float m { pieces_mat[f] };
		material += m;
		abs_material += fabs(m);
	}
@end(globals)
```
* add piece `f` to material count

```
@add(globals)
	void sub(signed char f) {
		float m { pieces_mat[f] };
		material -= m;
		abs_material -= fabs(m);
	}
@end(globals)
```
* remove piece `f` from material count

## Move Manually
* perform direct moves

```
@add(globals)
	bool is_pos(const char *p) {
		if (p[0] < 'a' || p[0] > 'h') {
			return false;
		}
		if (p[1] < '1' || p[1] > '8') {
			return false;
		}
		return true;
	}
@end(globals)
```
* a position consists of two characters
* the first is the rank and the second the file

```
@add(do cmd) {
	const char *cs { cmd.c_str() };
	if (cmd.size() == 4 &&
		is_pos(cs) && is_pos(cs + 2)
	) {
		@put(manual move);
		continue;
	}
} @end(do cmd)
```
* a move consists of two positions: the from field and the to field

```
@add(globals)
	int pos_to_idx(const char *p) {
		if (! is_pos(p)) { return 0; }
		int f { p[0] - 'a' };
		int r { p[1] - '1' };
		return f_r_to_idx(f, r);
	}
@end(globals)
```
* convert a position string to an index
* by converting the position to file and rank

```
@def(manual move)
	int from { pos_to_idx(cs) };
	int to { pos_to_idx(cs + 2) };
	if (from && to) {
		sub(board[to]);
		board[to] = board[from];
		board[from] = 0;
	}
@end(manual move)
```
* calculate the indices of the from and to fields
* if the positions are valid the material of the to field is subtracted
* then the piece is moved

## Clear Board
* note essential
* clears the board

```
@add(do cmd)
	if (cmd == "c") {
		@put(do clear);
		continue;
	}
@end(do cmd)
```
* the `c` command clears the board

```
@def(do clear)
	for (int r = 0; r <= 7; ++r) {
		for (int f = 0; f <= 7; ++f) {
			board[f_r_to_idx(f, r)] = 0;
		}
	}
	material = abs_material = 0.0f;
@end(do clear)
```
* set each field to the empty field
* and clear material counts

## Add Pieces
* not essential
* add black or white pieces
* can also clear fields

```
@add(do cmd)
	if (cmd == "w" || cmd == "b") {
		int mul = cmd == "w" ? 1 : -1;
		for (;;) {
			std::string piece;
			std::cin >> piece;
			if (piece == ".") { break; }
			@put(add piece);
			@mul(unknown piece);
		}
		continue;
	}
@end(do cmd)
```
* use the `w` for adding white pieces
* and `b` for adding black pieces
* afterwards specify the pieces to add
* terminate the sequence with `.`

```
@def(unknown piece)
	std::cerr << "unknown piece " <<
		piece << "\n";
@end(unknown piece)
```
* signal wrong piece syntax

```
@def(add piece) {
	const char *cs { piece.c_str() };
	if (piece.size() == 3 &&
		is_pos(cs + 1)
	) {
		@put(do add piece);
		continue;
	}
} @end(add piece)
```
* each piece starts with the kind of the piece
* and a position

```
@def(do add piece)
	int p { pos_to_idx(cs + 1) };
	sub(board[p]);
	switch (cs[0]) {
		case '.': board[p] = 0; break;
		@put(put other pieces);
		default:
			@mul(unknown piece);
	}
	add(board[p]);
@end(do add piece)
```
* a piece can be removed with `.` and the position

```
@def(put other pieces)
	case 'B': board[p] = mul * 1; break;
	case 'T': board[p] = mul * 2; break;
	case 'L': board[p] = mul * 3; break;
	case 'S': board[p] = mul * 4; break;
	case 'D': board[p] = mul * 5; break;
	case 'K': board[p] = mul * 6; break;
@end(put other pieces)
```
* use german uppercase letters to set the figure

## Add State
* keep the current state of the chess match

```
@add(setup)
	struct State {
		bool whs_turn { true };
		bool wh_o_o { true };
		bool wh_o_o_o { true };
		bool bl_o_o { true };
		bool bl_o_o_o { true };
		int e_p_field { 0 };
	} state;
@end(setup)
```
* keep the current state of the chess match

