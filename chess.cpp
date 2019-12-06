
#line 5 "chess.md"

	
#line 16 "chess.md"

	#include <iostream>

#line 50 "chess.md"

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

#line 69 "chess.md"

	signed char board[120] {
		
#line 81 "chess.md"

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

#line 71 "chess.md"

	};

#line 144 "chess.md"

	inline int f_r_to_idx(
		int f, int r
	) {
		return r * 10 + f + 21;
	}

#line 158 "chess.md"

	inline signed char get(
		int f, int r
	) {
		return board[f_r_to_idx(f, r)];
	}

#line 185 "chess.md"

	float material { 0.0f };
	float abs_material { 481.0f };

#line 203 "chess.md"

	const float _pieces_mat[] {
		-200.0f, -9.0f, -3.25f, -3.5f,
		-5.0f, -1.0f, 0.0f, 1.0f, 5.0f,
		3.5f, 3.25f, 9.0f, 200.0f
	};
	const float *pieces_mat {
		_pieces_mat + 6
	};

#line 218 "chess.md"

	#include <cmath>

#line 225 "chess.md"

	void add(signed char f) {
		float m { pieces_mat[f] };
		material += m;
		abs_material += fabs(m);
	}

#line 236 "chess.md"

	void sub(signed char f) {
		float m { pieces_mat[f] };
		material -= m;
		abs_material -= fabs(m);
	}

#line 250 "chess.md"

	bool is_pos(const char *p) {
		if (p[0] < 'a' || p[0] > 'h') {
			return false;
		}
		if (p[1] < '1' || p[1] > '8') {
			return false;
		}
		return true;
	}

#line 279 "chess.md"

	int pos_to_idx(const char *p) {
		if (! is_pos(p)) { return 0; }
		int f { p[0] - 'a' };
		int r { p[1] - '1' };
		return f_r_to_idx(f, r);
	}

#line 292 "chess.md"

	inline void set(
		int pos, signed char fig
	) {
		sub(board[pos]);
		board[pos] = fig;
		add(board[pos]);
	}

#line 306 "chess.md"

	
#line 335 "chess.md"

	struct State {
		bool whs_turn { true };
		
#line 697 "chess.md"

	bool wh_o_o { true };
	bool wh_o_o_o { true };
	bool bl_o_o { true };
	bool bl_o_o_o { true };
	int e_p_field { 0 };

#line 338 "chess.md"
;
	} state;

#line 307 "chess.md"
;
	inline void move(
		int from, int to
	) {
		if (from && to) {
			set(to, board[from]);
			set(from, 0);
			
#line 345 "chess.md"

	state.whs_turn = ! state.whs_turn;

#line 314 "chess.md"
;
		}
	}

#line 379 "chess.md"

	constexpr float ILLG { 2000.0f };
	constexpr float MATT { 1000.0f };
	constexpr float PATT {  500.0f };

#line 387 "chess.md"

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

#line 418 "chess.md"

	void add_if_valid(
		std::vector<int> &tos, int f, int t, int mul
	) {
		if (is_valid(f, t, mul)) {
			tos.push_back(t);
		}
	}

#line 430 "chess.md"

	void add_row(
		std::vector<int> &tos, int p, int mul, int d
	) {
		for (int x { d };; x += d) {
			add_if_valid(tos, p, p + x, mul);
			if (board[p + x] != 0) { break; }
		}
	}

#line 443 "chess.md"

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

#line 465 "chess.md"

	void moves(int from, std::vector<int> &tos) {
		
#line 473 "chess.md"

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

#line 467 "chess.md"
;
	}

#line 553 "chess.md"

	int best_move(
		int &from, int &to, bool move_for_wh,
		float &best_wh, float &best_bl
	) {
		
#line 576 "chess.md"

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

#line 558 "chess.md"
;
		return move_for_wh ? best_wh : best_bl;
	}

#line 6 "chess.md"

	int main() {
		
#line 102 "chess.md"

	const std::string _pieces_board[] {
		"♚", "♛", "♞", "♝", "♜",
		"♟", ".", "♙", "♖",
		"♗", "♘", "♕", "♔"
	};
	const std::string *pieces_board {
		_pieces_board + 6
	};

#line 8 "chess.md"
;
		
#line 23 "chess.md"

	for (;;) {
		std::cout << "?> ";
		std::string cmd;
		if (! (std::cin >> cmd)) {
			break;
		}
		
#line 117 "chess.md"

	if (cmd == "p") {
		std::cout << "\n";
		
#line 129 "chess.md"

	for (int r = 7; r >= 0; --r) {
		std::cout << (r + 1) << " ";
		
#line 169 "chess.md"
;
	for (int f = 0; f <= 7; ++f) {
		std::cout << pieces_board[
			get(f, r)
		];
	}

#line 132 "chess.md"
;
		std::cout << "\n";
	}
	std::cout << "  abcdefgh\n\n";

#line 194 "chess.md"

	std::cout << "  Material: " <<
		abs_material << " (" <<
		material << ")\n";

#line 351 "chess.md"

	std::cout << "  " <<
		(state.whs_turn ? "Weiß" :
			"Schwarz")  << " am Zug\n";

#line 120 "chess.md"
;
		std::cout << "\n";
		continue;
	}

#line 266 "chess.md"
 {
	const char *cs { cmd.c_str() };
	if (cmd.size() == 4 &&
		is_pos(cs) && is_pos(cs + 2)
	) {
		
#line 323 "chess.md"

	int from { pos_to_idx(cs) };
	int to { pos_to_idx(cs + 2) };
	move(from, to);

#line 271 "chess.md"
;
		continue;
	}
} 
#line 361 "chess.md"

	if (cmd == "m") {
		int from { 0 }, to { 0 };
		
#line 565 "chess.md"

	float best_w { -MATT };
	float best_b { MATT };
	best_move(
		from, to, state.whs_turn,
		best_w, best_b
	);

#line 364 "chess.md"
;
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

#line 598 "chess.md"

	if (cmd == "c") {
		
#line 608 "chess.md"

	for (int r = 0; r <= 7; ++r) {
		for (int f = 0; f <= 7; ++f) {
			board[f_r_to_idx(f, r)] = 0;
		}
	}
	material = abs_material = 0.0f;

#line 600 "chess.md"
;
		continue;
	}

#line 626 "chess.md"

	if (cmd == "w" || cmd == "b") {
		int mul = cmd == "w" ? 1 : -1;
		for (;;) {
			std::string piece;
			std::cin >> piece;
			if (piece == ".") { break; }
			
#line 654 "chess.md"
 {
	const char *cs { piece.c_str() };
	if (piece.size() == 3 &&
		is_pos(cs + 1)
	) {
		
#line 668 "chess.md"

	int p { pos_to_idx(cs + 1) };
	switch (cs[0]) {
		case '.': set(p, 0); break;
		
#line 682 "chess.md"

	case 'B': set(p, mul * 1); break;
	case 'T': set(p, mul * 2); break;
	case 'L': set(p, mul * 3); break;
	case 'S': set(p, mul * 4); break;
	case 'D': set(p, mul * 5); break;
	case 'K': set(p, mul * 6); break;

#line 672 "chess.md"
;
		default:
			
#line 646 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 674 "chess.md"
;
	}
	add(board[p]);

#line 659 "chess.md"
;
		continue;
	}
} 
#line 633 "chess.md"
;
			
#line 646 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 634 "chess.md"
;
		}
		continue;
	}

#line 30 "chess.md"
;
		
#line 39 "chess.md"

	std::cerr << "unknown command " <<
		cmd << "\n";

#line 31 "chess.md"
;
	}

#line 9 "chess.md"
;
	}
