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
		int color { 1 };
		@put(state init);
	} state;
@end(needed by move)
```
* keep the current state of the chess match

```
@def(switch color)
	state.color = -state.color;
@end(switch color)
```
* switch playing color between `1` (white) and `-1` (black)

```
@add(print board)
	std::cout << "  " <<
		(state.color > 0 ? "Weiß" :
			"Schwarz")  << " am Zug\n";
@end(print board)
```
* print which color is playing

## Calculate a move
* the computer calculates the next move

```
@add(do cmd)
	if (cmd == "m") {
		int from { 0 }, to { 0 };
		@put(calc move);
		if (from && to) {
			move(from, to);
			@put(print move);
		} else {
			std::cout << "no move\n";
		}
		continue;
	}
@end(do cmd)
```
* the `m` command computes a computer move
* it is performed and printed
* if no move is found an error message is printed instead

```
@add(globals)
	void idx_to_pos(char *pos, int idx) {
		pos[0] = idx % 10 + 'a' - 1;
		pos[1] = idx / 10 + '1' - 2;
		pos[2] = '\0';
	}
@end(globals)
```
* convert a valid index position to a string

```
@def(print move)
	char pos[3];
	idx_to_pos(pos, from);
	std::cout << pos;
	idx_to_pos(pos, to);
	std::cout << pos << "\n";
@end(print move)
```
* print from and to positions

```
@add(globals)
	@put(best move prereqs);
	int best_move(
		int &from, int &to, int color,
		float &best_wh, float &best_bl
	) {
		@put(best move);
		return color > 0 ?
			best_wh : best_bl;
	}
@end(globals)
```
* calculate the best move for a given color
* current best values for white and black are kept separatedly

```
@add(globals)
	#include <limits>
	using nlf =
		std::numeric_limits<float>;
@end(globals)
```
* needs `std::numeric_limits<float>`

```
@def(calc move)
	constexpr float inf {
		nlf::infinity()
	};
	float best_w { -1 * inf };
	float best_b { inf };
	best_move(
		from, to, state.color,
		best_w, best_b
	);
@end(calc move)
```
* initialize values with extremly conservative values
* so any move found will top these values

```
@def(best move)
	for (int i = 0; i < 120; ++i) {
		if (board[i] != X &&
			board[i] * color > 0
		) {
			@put(check for best move);
		}
	}
@end(best move)
```
* no move can origin from a boarder field
* neither from a field that has not a figure of the needed color
* go into more specific checking for all other fields

```
@def(moves prereqs)
	@put(pseudo moves prereqs)
	template<typename I>
	void pseudo_moves(
		int f, const I &it
	) {
		@put(pseudo moves);
	}
@end(moves prereqs)
```
* iterate over all pseudo-legal moves
* that is all moves possible without checking if after that move the
  own king is still in check

```
@def(best move prereqs)
	@put(moves prereqs)
	template<typename I>
	void moves(int f, const I &it) {
		int c { board[f] > 0 ? 1 : -1 };
		pseudo_moves(f, [&](int t) {
			@put(check for check);
		});
	}
@end(best move prereqs)
```
* iterate over all valid moves
* for each pseudo-legal move the function checks if the move will place
  (or keep) the own king in check

```
@add(best move prereqs)
	bool eval_move(
		int from, int to, int color,
		float &best_wh, float &best_bl
	) {
		bool better { false };
		@put(eval move);
		return better;
	}
@end(best move prereqs)
```
* get a value for a move
* higher values are better for white
* lower values are better for black
* returns `true` if the move has improved `best_wh` or `best_bl`

```
@def(check for best move)
	moves(i, [&](int m) {
		if (eval_move(
			i, m, color,
			best_wh, best_bl
		)) {
			from = i; to = m;
		}
	});
@end(check for best move)
```
* iterate over all legal moves
* if it is an improvement, save the move

```
@def(eval move)
	signed char old { board[to] };
	move(from, to);
	@put(check for improvement);
	move(to, from);
	set(to, old);
@end(eval move)
```
* perform the move but save the origin constellation
* check if the new situation is an improvement
* undo the move at the end

```
@def(check for improvement)
	if (color > 0 &&
		material > best_wh
	) {
		best_wh = material;
		better = true;
	}
@end(check for improvement)
```
* an improvement if white is playing

```
@add(check for improvement)
	if (color < 0 &&
		material < best_bl
	) {
		best_bl = material;
		better = true;
	}
@end(check for improvement)
```
* an improvement if black is playing

```
@def(pseudo moves)
	auto p { board[f] };
	if (p == EE || p == X) { return; }
	int c { p > 0 ? 1 : -1 };
	@put(pseudo moves for piece);
@end(pseudo moves)
```
* no move origins from an empty field or the boarder
* the color of the origin field is saved
* calculate the moves depending on the figure type

```
@def(pseudo moves prereqs)
	bool is_pseudo_valid(
		int f, int t, int color
	) {
		auto x { board[t] };
		if (x == X) { return false; }
		if (x * color > 0) {
			return false;
		}
		return true;
	}
@end(pseudo moves prereqs)
```
* a move is pseudo-valid if the target field is not on the boarder
* and not populated by a figure of the same color

```
@add(pseudo moves prereqs)
	template<typename I>
	void add_if_valid(
		const I &it, int f, int t, int c
	) {
		if (is_pseudo_valid(f, t, c)) {
			it(t);
		}
	}
@end(pseudo moves prereqs)
```
* process the move only if it is pseudo-valid

```
@add(pseudo moves prereqs)
	template<typename I>
	void add_row(
		const I &it, int f, int c, int d
	) {
		for (int x { d };; x += d) {
			add_if_valid(it, f, f + x, c);
			if (board[f + x] != 0) {
				break;
			}
		}
	}
@end(pseudo moves prereqs)
```
* iterates over a row of fields starting at `f`
* the iteration stops if a non-empty field is processed

```
@def(pseudo moves for piece)
	if (p == WS || p == BS) {
		add_if_valid(it, f, f - 21, c);
		add_if_valid(it, f, f - 19, c);
		add_if_valid(it, f, f - 12, c);
		add_if_valid(it, f, f - 8, c);
		add_if_valid(it, f, f + 8, c);
		add_if_valid(it, f, f + 12, c);
		add_if_valid(it, f, f + 19, c);
		add_if_valid(it, f, f + 21, c);
	}
@end(pseudo moves for piece)
```
* the knight can move to eight possible fields

```
@add(pseudo moves for piece)
	if (p == WP) {
		@put(white pawn moves);
	} else if (p == BP) {
		@put(black pawn moves);
	}
@end(pseudo moves for piece)
```
* pawns are more complicated
* special treatment for white and black pawns

```
@def(white pawn moves)
	if (board[f + 10] == 0) {
		add_if_valid(it, f, f - 10, c);
	}
	if (board[f + 9] * c < 0) {
		add_if_valid(it, f, f + 9, c);
	}
	if (board[f + 11] * c < 0) {
		add_if_valid(it, f, f + 11, c);
	}
@end(white pawn moves)
```
* white pawns can move forward if the field is empty
* white pawns can take other figures on the diagonal

```
@add(white pawn moves)
	int r { f / 10 - 2 };
	if (r == 1 && board[f + 10] == 0 &&
		board[f + 20] == 0
	) {
		add_if_valid(it, f, f + 20, c);
	}
@end(white pawn moves)
```
* if a white pawn is on the origin rank it can move two ranks forward
  if these two fields are empty

```
@def(black pawn moves)
	if (board[f - 10] == 0) {
		add_if_valid(it, f, f - 10, c);
	}
	if (board[f - 9] * c < 0) {
		add_if_valid(it, f, f - 9, c);
	}
	if (board[f - 11] * c < 0) {
		add_if_valid(it, f, f - 11, c);
	}
@end(black pawn moves)
```
* black pawns can move downward if the field is empty
* black pawns can take other figures on the diagonal

```
@add(black pawn moves)
	int r { f / 10 - 2 };
	if (r == 6 && board[f - 10] == 0 &&
		board[f - 20] == 0
	) {
		add_if_valid(it, f, f - 20, c);
	}
@end(black pawn moves)
```
* if a black pawn is on the origin rank it can move two ranks downward
  if these two fields are empty

```
@add(pseudo moves for piece)
	if (p == WR || p == BR) {
		add_row(it, f, c, -10);
		add_row(it, f, c, -1);
		add_row(it, f, c, 1);
		add_row(it, f, c, 10);
	}
@end(pseudo moves for piece)
```
* rooks move on ranks or files

```
@add(pseudo moves for piece)
	if (p == WN || p == BN) {
		add_row(it, f, c, -11);
		add_row(it, f, c, -9);
		add_row(it, f, c, 9);
		add_row(it, f, c, 11);
	}
@end(pseudo moves for piece)
```
* bishops move on diagonals

```
@add(pseudo moves for piece)
	if (p == WQ || p == BQ) {
		add_row(it, f, c, -11);
		add_row(it, f, c, -10);
		add_row(it, f, c, -9);
		add_row(it, f, c, -1);
		add_row(it, f, c, 1);
		add_row(it, f, c, 10);
		add_row(it, f, c, 9);
		add_row(it, f, c, 11);
	}
@end(pseudo moves for piece)
```
* queens can move on ranks, files and diagonals

```
@add(pseudo moves for piece)
	if (p == WK || p == BK) {
		add_if_valid(it, f, f - 11, c);
		add_if_valid(it, f, f - 10, c);
		add_if_valid(it, f, f - 9, c);
		add_if_valid(it, f, f - 1, c);
		add_if_valid(it, f, f + 1, c);
		add_if_valid(it, f, f + 9, c);
		add_if_valid(it, f, f + 10, c);
		add_if_valid(it, f, f + 11, c);
	}
@end(pseudo moves for piece)
```
* kings can move one field in any direction

```
@def(check for check)
	auto x { board[t] };
	move(f, t);
	bool in_check { false };
	for (int i = 0; i < 120; ++i) {
		if (board[i] * c < 0) {
			@put(attacks king?)
		}
	}
	move(t, f);
	set(t, x);
	if (! in_check) { it(t); }
@end(check for check)
```
* perform the move
* then see if any figure of the opposite color can take the own king

```
@def(attacks king?)
	pseudo_moves(i, [&](int m) {
		if (board[m] == WK * c) {
			in_check = true;
		}
	});
	if (in_check) { break; }
@end(attacks king?)
```
* can the own king be taken?
* if so no reason to continue the loop

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
		int color { cmd == "w" ? 1 : -1 };
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
	case 'B': set(p, color * 1); break;
	case 'T': set(p, color * 2); break;
	case 'L': set(p, color * 3); break;
	case 'S': set(p, color * 4); break;
	case 'D': set(p, color * 5); break;
	case 'K': set(p, color * 6); break;
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

