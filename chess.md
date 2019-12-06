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
	constexpr signed char X { 100 };
	constexpr signed char EE { 0 };
	constexpr signed char WP { 1 };
	constexpr signed char WR { 2 };
	constexpr signed char WN { 3 };
	constexpr signed char WS { 4 };
	constexpr signed char WQ { 5 };
	constexpr signed char WK { 6 };
	constexpr signed char BP { -1 };
	constexpr signed char BR { -2 };
	constexpr signed char BN { -3 };
	constexpr signed char BS { -4 };
	constexpr signed char BQ { -5 };
	constexpr signed char BK { -6 };
@end(globals)
```

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
* the boarder is initialized with 100 

```
@def(initial board)
	X,  X,  X,  X,  X,  X,  X,  X,  X, X,
	X,  X,  X,  X,  X,  X,  X,  X,  X, X,
	X, WR, WS, WN, WQ, WK, WN, WS, WR, X,
	X, WP, WP, WP, WP, WP, WP, WP, WP, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, BP, BP, BP, BP, BP, BP, BP, BP, X,
	X, BR, BS, BN, BQ, BK, BN, BS, BR, X,
	X,  X,  X,  X,  X,  X,  X,  X,  X, X,
	X,  X,  X,  X,  X,  X,  X,  X,  X, X
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
		std::cout << "\n";
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
		material << ")\n";
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
@add(globals)
	inline void set(
		int pos, signed char fig
	) {
		sub(board[pos]);
		board[pos] = fig;
		add(board[pos]);
	}
@end(globals)
```
* set figure on field
* and adjust material values

```
@add(globals)
	@put(needed by move);
	inline void move(
		int from, int to
	) {
		if (from && to) {
			set(to, board[from]);
			set(from, 0);
			@put(switch color);
		}
	}
@end(globals)
```
* if the positions are valid the material of the to field is subtracted
* then the piece is moved

```
@def(manual move)
	int from { pos_to_idx(cs) };
	int to { pos_to_idx(cs + 2) };
	move(from, to);
@end(manual move)
```
* calculate the indices of the from and to fields
* and perform move

## Switch Move Colors

```
@def(needed by move)
	struct State {
		bool whs_turn { true };
		@put(state init);
	} state;
@end(needed by move)
```
* keep the current state of the chess match

```
@def(switch color)
	state.whs_turn = ! state.whs_turn;
@end(switch color)
```

```
@add(print board)
	std::cout << "  " <<
		(state.whs_turn ? "Weiß" :
			"Schwarz")  << " am Zug\n";
@end(print board)
```

## Calculate a move

```
@def(do cmd)
	if (cmd == "m") {
		int from { 0 }, to { 0 };
		@put(calc move);
		if (from && to) {
			move(from, to);
			std::cout << (char) (from % 10 + 'a' - 1) << (char) (from / 10 + '1' - 2);
			std::cout << (char) (to % 10 + 'a' - 1) << (char) (to / 10 + '1' - 2);
			std::cout << "\n";
		} else {
			std::cout << "no move\n";
		}
		continue;
	}
@end(do cmd)
```

```
@add(globals)
	constexpr float ILLG { 2000.0f };
	constexpr float MATT { 1000.0f };
	constexpr float PATT {  500.0f };
@end(globals)
```

```
@add(globals)
	#include <vector>
	void moves(int from, std::vector<int> &tos);
	bool is_valid(int f, int t, int mul) {
		auto x { board[t] };
		if (x == X) { return false; }
		if (x * mul > 0) {
			return false;
		}
		static bool shortcut { false };
		if (shortcut) { return true; }
		shortcut = true;
		move(f, t);
		for (int i = 0; i < 120; ++i) {
			if (board[i] * mul < 0) {
				std::vector<int> mvs;
				moves(i, mvs);
				for (int m : mvs) {
					if (board[m] == WK * mul) { return false; }
				}
			}
		}
		move(t, f);
		set(t, x);
		shortcut = false;
		return true;
	}
@end(globals)
```

```
@add(globals)
	void add_if_valid(
		std::vector<int> &tos, int f, int t, int mul
	) {
		if (is_valid(f, t, mul)) {
			tos.push_back(t);
		}
	}
@end(globals)
```

```
@add(globals)
	void add_row(
		std::vector<int> &tos, int p, int mul, int d
	) {
		for (int x { d };; x += d) {
			add_if_valid(tos, p, p + x, mul);
			if (board[p + x] != 0) { break; }
		}
	}
@end(globals)
```

```
@add(globals)
	bool eval_move(int from, int to, bool wh_moves, float &best_wh, float &best_bl) {
		signed char old { board[to] };
		move(from, to);
		bool better { false };
		if (wh_moves && material > best_wh) {
			best_wh = material;
			better = true;
		}
		if (! wh_moves && material < best_bl) {
			best_bl = material;
			better = true;
		}
		move(to, from);
		board[to] = old;
		set(to, old);
		return better;
	}
@end(globals)
```

```
@add(globals)
	void moves(int from, std::vector<int> &tos) {
		@put(moves);
	}
@end(globals)
```

```
@def(moves)
	auto p { board[from] };
	if (p == EE || p == X) { return; }
	int mul { p > 0 ? 1 : -1 };
	if (p == WS || p == BS) {
		add_if_valid(tos, from, from - 21, mul);
		add_if_valid(tos, from, from - 19, mul);
		add_if_valid(tos, from, from - 12, mul);
		add_if_valid(tos, from, from - 8, mul);
		add_if_valid(tos, from, from + 8, mul);
		add_if_valid(tos, from, from + 12, mul);
		add_if_valid(tos, from, from + 19, mul);
		add_if_valid(tos, from, from + 21, mul);
	}
	if (p == WP) {
		if (board[from + 10] == 0) {
			add_if_valid(tos, from, from - 10, mul);
		}
		if (board[from + 9] * mul < 0) {
			add_if_valid(tos, from, from + 9, mul);
		}
		if (board[from + 11] * mul < 0) {
			add_if_valid(tos, from, from + 11, mul);
		}
		int r { from / 10 - 2 };
		if (r == 1 && board[from + 10] == 0 && board[from + 20] == 0) {
			add_if_valid(tos, from, from + 20, mul);
		}
	}
	if (p == BP) {
		if (board[from - 10] == 0) {
			add_if_valid(tos, from, from - 10, mul);
		}
		if (board[from - 9] * mul < 0) {
			add_if_valid(tos, from, from - 9, mul);
		}
		if (board[from - 11] * mul < 0) {
			add_if_valid(tos, from, from - 11, mul);
		}
		int r { from / 10 - 2 };
		if (r == 6 && board[from - 10] == 0 && board[from - 20] == 0) {
			add_if_valid(tos, from, from - 20, mul);
		}
	}
	if (p == WR || p == BR) {
		add_row(tos, from, mul, -10);
		add_row(tos, from, mul, -1);
		add_row(tos, from, mul, 1);
		add_row(tos, from, mul, 10);
	}
	if (p == WN || p == BN) {
		add_row(tos, from, mul, -11);
		add_row(tos, from, mul, -9);
		add_row(tos, from, mul, 9);
		add_row(tos, from, mul, 11);
	}
	if (p == WQ || p == BQ) {
		add_row(tos, from, mul, -11);
		add_row(tos, from, mul, -10);
		add_row(tos, from, mul, -9);
		add_row(tos, from, mul, -1);
		add_row(tos, from, mul, 1);
		add_row(tos, from, mul, 10);
		add_row(tos, from, mul, 9);
		add_row(tos, from, mul, 11);
	}
	if (p == WK || p == BK) {
		add_if_valid(tos, from, from - 11, mul);
		add_if_valid(tos, from, from - 10, mul);
		add_if_valid(tos, from, from - 9, mul);
		add_if_valid(tos, from, from - 1, mul);
		add_if_valid(tos, from, from + 1, mul);
		add_if_valid(tos, from, from + 9, mul);
		add_if_valid(tos, from, from + 10, mul);
		add_if_valid(tos, from, from + 11, mul);
	}
@end(moves)
```

```
@add(globals)
	int best_move(
		int &from, int &to, bool move_for_wh,
		float &best_wh, float &best_bl
	) {
		@put(best move);
		return move_for_wh ? best_wh : best_bl;
	}
@end(globals)
```

```
@def(calc move)
	float best_w { -MATT };
	float best_b { MATT };
	best_move(
		from, to, state.whs_turn,
		best_w, best_b
	);
@end(calc move)
```

```
@def(best move)
	int mul { move_for_wh ? 1 : -1 };
	for (int i = 0; i < 120; ++i) {
		if (board[i] * mul > 0) {
			std::vector<int> mvs;
			moves(i, mvs);
			for (int m : mvs) {
				if (eval_move(i, m, move_for_wh, best_wh, best_bl)) {
					from = i;
					to = m;
				}
			}
		}
	}
@end(best move)
```

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
	switch (cs[0]) {
		case '.': set(p, 0); break;
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
	case 'B': set(p, mul * 1); break;
	case 'T': set(p, mul * 2); break;
	case 'L': set(p, mul * 3); break;
	case 'S': set(p, mul * 4); break;
	case 'D': set(p, mul * 5); break;
	case 'K': set(p, mul * 6); break;
@end(put other pieces)
```
* use german uppercase letters to set the figure

## Add State
* keep the current state of the chess match

```
@def(state init)
	bool wh_o_o { true };
	bool wh_o_o_o { true };
	bool bl_o_o { true };
	bool bl_o_o_o { true };
	int e_p_field { 0 };
@end(state init)
```
* keep the current state of the chess match

