
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
		int color { 1 };
		
#line 898 "chess.md"

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

	state.color = -state.color;

#line 314 "chess.md"
;
		}
	}

#line 383 "chess.md"

	void idx_to_pos(char *pos, int idx) {
		pos[0] = idx % 10 + 'a' - 1;
		pos[1] = idx / 10 + '1' - 2;
		pos[2] = '\0';
	}

#line 405 "chess.md"

	
#line 476 "chess.md"

	
#line 461 "chess.md"

	
#line 573 "chess.md"

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

#line 590 "chess.md"

	template<typename I>
	void add_if_valid(
		const I &it, int f, int t, int c
	) {
		if (is_pseudo_valid(f, t, c)) {
			it(t);
		}
	}

#line 604 "chess.md"

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

#line 462 "chess.md"

	template<typename I>
	void pseudo_moves(
		int f, const I &it
	) {
		
#line 561 "chess.md"

	auto p { board[f] };
	if (p == EE || p == X) { return; }
	int c { p > 0 ? 1 : -1 };
	
#line 622 "chess.md"

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

#line 638 "chess.md"

	if (p == WP) {
		
#line 650 "chess.md"

	if (board[f + 10] == 0) {
		add_if_valid(it, f, f - 10, c);
	}
	if (board[f + 9] * c < 0) {
		add_if_valid(it, f, f + 9, c);
	}
	if (board[f + 11] * c < 0) {
		add_if_valid(it, f, f + 11, c);
	}

#line 666 "chess.md"

	int r { f / 10 - 2 };
	if (r == 1 && board[f + 10] == 0 &&
		board[f + 20] == 0
	) {
		add_if_valid(it, f, f + 20, c);
	}

#line 640 "chess.md"
;
	} else if (p == BP) {
		
#line 679 "chess.md"

	if (board[f - 10] == 0) {
		add_if_valid(it, f, f - 10, c);
	}
	if (board[f - 9] * c < 0) {
		add_if_valid(it, f, f - 9, c);
	}
	if (board[f - 11] * c < 0) {
		add_if_valid(it, f, f - 11, c);
	}

#line 695 "chess.md"

	int r { f / 10 - 2 };
	if (r == 6 && board[f - 10] == 0 &&
		board[f - 20] == 0
	) {
		add_if_valid(it, f, f - 20, c);
	}

#line 642 "chess.md"
;
	}

#line 708 "chess.md"

	if (p == WR || p == BR) {
		add_row(it, f, c, -10);
		add_row(it, f, c, -1);
		add_row(it, f, c, 1);
		add_row(it, f, c, 10);
	}

#line 720 "chess.md"

	if (p == WN || p == BN) {
		add_row(it, f, c, -11);
		add_row(it, f, c, -9);
		add_row(it, f, c, 9);
		add_row(it, f, c, 11);
	}

#line 732 "chess.md"

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

#line 748 "chess.md"

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

#line 565 "chess.md"
;

#line 467 "chess.md"
;
	}

#line 477 "chess.md"

	template<typename I>
	void moves(int f, const I &it) {
		int c { board[f] > 0 ? 1 : -1 };
		pseudo_moves(f, [&](int t) {
			
#line 764 "chess.md"

	auto x { board[t] };
	move(f, t);
	bool in_check { false };
	for (int i = 0; i < 120; ++i) {
		if (board[i] * c < 0) {
			
#line 782 "chess.md"

	pseudo_moves(i, [&](int m) {
		if (board[m] == WK * c) {
			in_check = true;
		}
	});
	if (in_check) { break; }

#line 770 "chess.md"

		}
	}
	move(t, f);
	set(t, x);
	if (! in_check) { it(t); }

#line 482 "chess.md"
;
		});
	}

#line 492 "chess.md"

	bool eval_move(
		int from, int to, int color,
		float &best_wh, float &best_bl
	) {
		bool better { false };
		
#line 524 "chess.md"

	signed char old { board[to] };
	move(from, to);
	
#line 537 "chess.md"

	if (color > 0 &&
		material > best_wh
	) {
		best_wh = material;
		better = true;
	}

#line 549 "chess.md"

	if (color < 0 &&
		material < best_bl
	) {
		best_bl = material;
		better = true;
	}

#line 527 "chess.md"
;
	move(to, from);
	set(to, old);

#line 498 "chess.md"
;
		return better;
	}

#line 406 "chess.md"
;
	int best_move(
		int &from, int &to, int color,
		float &best_wh, float &best_bl
	) {
		
#line 446 "chess.md"

	for (int i = 0; i < 120; ++i) {
		if (board[i] != X &&
			board[i] * color > 0
		) {
			
#line 509 "chess.md"

	moves(i, [&](int m) {
		if (eval_move(
			i, m, color,
			best_wh, best_bl
		)) {
			from = i; to = m;
		}
	});

#line 451 "chess.md"
;
		}
	}

#line 411 "chess.md"
;
		return color > 0 ?
			best_wh : best_bl;
	}

#line 421 "chess.md"

	#include <limits>
	using nlf =
		std::numeric_limits<float>;

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

#line 352 "chess.md"

	std::cout << "  " <<
		(state.color > 0 ? "Weiß" :
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
#line 364 "chess.md"

	if (cmd == "m") {
		int from { 0 }, to { 0 };
		
#line 430 "chess.md"

	constexpr float inf {
		nlf::infinity()
	};
	float best_w { -1 * inf };
	float best_b { inf };
	best_move(
		from, to, state.color,
		best_w, best_b
	);

#line 367 "chess.md"
;
		if (from && to) {
			move(from, to);
			
#line 394 "chess.md"

	char pos[3];
	idx_to_pos(pos, from);
	std::cout << pos;
	idx_to_pos(pos, to);
	std::cout << pos << "\n";

#line 370 "chess.md"
;
		} else {
			std::cout << "no move\n";
		}
		continue;
	}

#line 799 "chess.md"

	if (cmd == "c") {
		
#line 809 "chess.md"

	for (int r = 0; r <= 7; ++r) {
		for (int f = 0; f <= 7; ++f) {
			board[f_r_to_idx(f, r)] = 0;
		}
	}
	material = abs_material = 0.0f;

#line 801 "chess.md"
;
		continue;
	}

#line 827 "chess.md"

	if (cmd == "w" || cmd == "b") {
		int color { cmd == "w" ? 1 : -1 };
		for (;;) {
			std::string piece;
			std::cin >> piece;
			if (piece == ".") { break; }
			
#line 855 "chess.md"
 {
	const char *cs { piece.c_str() };
	if (piece.size() == 3 &&
		is_pos(cs + 1)
	) {
		
#line 869 "chess.md"

	int p { pos_to_idx(cs + 1) };
	switch (cs[0]) {
		case '.': set(p, 0); break;
		
#line 883 "chess.md"

	case 'B': set(p, color * 1); break;
	case 'T': set(p, color * 2); break;
	case 'L': set(p, color * 3); break;
	case 'S': set(p, color * 4); break;
	case 'D': set(p, color * 5); break;
	case 'K': set(p, color * 6); break;

#line 873 "chess.md"
;
		default:
			
#line 847 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 875 "chess.md"
;
	}
	add(board[p]);

#line 860 "chess.md"
;
		continue;
	}
} 
#line 834 "chess.md"
;
			
#line 847 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 835 "chess.md"
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
