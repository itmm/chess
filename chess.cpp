
#line 5 "chess.md"

	
#line 16 "chess.md"

	#include <iostream>

#line 50 "chess.md"

	
#line 63 "chess.md"

	constexpr signed char XX { 100 };
	constexpr signed char X { XX };
	constexpr signed char EE { 0 };

#line 74 "chess.md"

	constexpr signed char WP { 1 };
	constexpr signed char WR { 2 };
	constexpr signed char WB { 3 };
	constexpr signed char WN { 4 };
	constexpr signed char WQ { 5 };
	constexpr signed char WK { 6 };

#line 86 "chess.md"

	constexpr signed char BP { -1 };
	constexpr signed char BR { -2 };
	constexpr signed char BB { -3 };
	constexpr signed char BN { -4 };
	constexpr signed char BQ { -5 };
	constexpr signed char BK { -6 };

#line 51 "chess.md"
;
	signed char board[120] {
		
#line 99 "chess.md"

	X, XX, XX, XX, XX, XX, XX, XX, XX, X,
	X, XX, XX, XX, XX, XX, XX, XX, XX, X,
	X, WR, WN, WB, WQ, WK, WB, WN, WR, X,
	X, WP, WP, WP, WP, WP, WP, WP, WP, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, EE, EE, EE, EE, EE, EE, EE, EE, X,
	X, BP, BP, BP, BP, BP, BP, BP, BP, X,
	X, BR, BN, BB, BQ, BK, BB, BN, BR, X,
	X, XX, XX, XX, XX, XX, XX, XX, XX, X,
	X, XX, XX, XX, XX, XX, XX, XX, XX, X

#line 53 "chess.md"

	};

#line 162 "chess.md"

	inline int f_r_to_idx(
		int f, int r
	) {
		return r * 10 + f + 21;
	}

#line 176 "chess.md"

	inline signed char get(
		int f, int r
	) {
		return board[f_r_to_idx(f, r)];
	}

#line 203 "chess.md"

	float material { 0.0f };
	float abs_material { 481.0f };

#line 221 "chess.md"

	const float _pieces_mat[] {
		-200.0f, -9.0f, -3.25f, -3.5f,
		-5.0f, -1.0f, 0.0f, 1.0f, 5.0f,
		3.5f, 3.25f, 9.0f, 200.0f
	};
	const float *pieces_mat {
		_pieces_mat + 6
	};

#line 236 "chess.md"

	#include <cmath>

#line 243 "chess.md"

	void add(signed char f) {
		float m { pieces_mat[f] };
		material += m;
		abs_material += fabs(m);
	}

#line 254 "chess.md"

	void sub(signed char f) {
		float m { pieces_mat[f] };
		material -= m;
		abs_material -= fabs(m);
	}

#line 268 "chess.md"

	bool is_pos(const char *p) {
		if (p[0] < 'a' || p[0] > 'h') {
			return false;
		}
		if (p[1] < '1' || p[1] > '8') {
			return false;
		}
		return true;
	}

#line 297 "chess.md"

	int pos_to_idx(const char *p) {
		if (! is_pos(p)) { return 0; }
		int f { p[0] - 'a' };
		int r { p[1] - '1' };
		return f_r_to_idx(f, r);
	}

#line 310 "chess.md"

	inline void set(
		int pos, signed char fig
	) {
		sub(board[pos]);
		board[pos] = fig;
		add(board[pos]);
	}

#line 324 "chess.md"

	
#line 354 "chess.md"

	struct State {
		int color { 1 };
		
#line 927 "chess.md"

	bool wh_o_o { true };
	bool wh_o_o_o { true };
	bool bl_o_o { true };
	bool bl_o_o_o { true };
	int e_p_field { 0 };

#line 357 "chess.md"
;
	} state;

#line 325 "chess.md"
;
	inline void move(
		int from, int to
	) {
		if (from && to) {
			set(to, board[from]);
			set(from, 0);
			
#line 364 "chess.md"

	state.color = -state.color;

#line 332 "chess.md"
;
		}
	}

#line 402 "chess.md"

	void idx_to_pos(char *pos, int idx) {
		pos[0] = idx % 10 + 'a' - 1;
		pos[1] = idx / 10 + '1' - 2;
		pos[2] = '\0';
	}

#line 424 "chess.md"

	
#line 495 "chess.md"

	
#line 480 "chess.md"

	
#line 592 "chess.md"

	bool is_pseudo_valid(
		int f, int t, int color
	) {
		auto x { board[t] };
		if (x == XX) { return false; }
		if (x * color > 0) {
			return false;
		}
		return true;
	}

#line 609 "chess.md"

	template<typename I>
	void add_if_valid(
		const I &it, int f, int t, int c
	) {
		if (is_pseudo_valid(f, t, c)) {
			it(t);
		}
	}

#line 623 "chess.md"

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

#line 481 "chess.md"

	template<typename I>
	void pseudo_moves(
		int f, const I &it
	) {
		
#line 580 "chess.md"

	auto p { board[f] };
	if (p == EE || p == XX) { return; }
	int c { p > 0 ? 1 : -1 };
	
#line 641 "chess.md"

	if (p == WN || p == BN) {
		add_if_valid(it, f, f - 21, c);
		add_if_valid(it, f, f - 19, c);
		add_if_valid(it, f, f - 12, c);
		add_if_valid(it, f, f - 8, c);
		add_if_valid(it, f, f + 8, c);
		add_if_valid(it, f, f + 12, c);
		add_if_valid(it, f, f + 19, c);
		add_if_valid(it, f, f + 21, c);
	}

#line 657 "chess.md"

	if (p == WP) {
		
#line 669 "chess.md"

	if (board[f + 10] == 0) {
		add_if_valid(it, f, f - 10, c);
	}
	if (board[f + 9] * c < 0) {
		add_if_valid(it, f, f + 9, c);
	}
	if (board[f + 11] * c < 0) {
		add_if_valid(it, f, f + 11, c);
	}

#line 685 "chess.md"

	int r { f / 10 - 2 };
	if (r == 1 && board[f + 10] == 0 &&
		board[f + 20] == 0
	) {
		add_if_valid(it, f, f + 20, c);
	}

#line 659 "chess.md"
;
	} else if (p == BP) {
		
#line 698 "chess.md"

	if (board[f - 10] == 0) {
		add_if_valid(it, f, f - 10, c);
	}
	if (board[f - 9] * c < 0) {
		add_if_valid(it, f, f - 9, c);
	}
	if (board[f - 11] * c < 0) {
		add_if_valid(it, f, f - 11, c);
	}

#line 714 "chess.md"

	int r { f / 10 - 2 };
	if (r == 6 && board[f - 10] == 0 &&
		board[f - 20] == 0
	) {
		add_if_valid(it, f, f - 20, c);
	}

#line 661 "chess.md"
;
	}

#line 727 "chess.md"

	if (p == WR || p == BR) {
		add_row(it, f, c, -10);
		add_row(it, f, c, -1);
		add_row(it, f, c, 1);
		add_row(it, f, c, 10);
	}

#line 739 "chess.md"

	if (p == WB || p == BB) {
		add_row(it, f, c, -11);
		add_row(it, f, c, -9);
		add_row(it, f, c, 9);
		add_row(it, f, c, 11);
	}

#line 751 "chess.md"

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

#line 767 "chess.md"

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

#line 584 "chess.md"
;

#line 486 "chess.md"
;
	}

#line 496 "chess.md"

	template<typename I>
	void moves(int f, const I &it) {
		int c { board[f] > 0 ? 1 : -1 };
		pseudo_moves(f, [&](int t) {
			
#line 783 "chess.md"

	auto x { board[t] };
	move(f, t);
	bool in_check { false };
	for (int i = 0; i < 120; ++i) {
		if (board[i] * c < 0) {
			
#line 801 "chess.md"

	pseudo_moves(i, [&](int m) {
		if (board[m] == WK * c) {
			in_check = true;
		}
	});
	if (in_check) { break; }

#line 789 "chess.md"

		}
	}
	move(t, f);
	set(t, x);
	if (! in_check) { it(t); }

#line 501 "chess.md"
;
		});
	}

#line 511 "chess.md"

	bool eval_move(
		int from, int to, int color,
		float &best_wh, float &best_bl
	) {
		bool better { false };
		
#line 543 "chess.md"

	signed char old { board[to] };
	move(from, to);
	
#line 556 "chess.md"

	if (color > 0 &&
		material > best_wh
	) {
		best_wh = material;
		better = true;
	}

#line 568 "chess.md"

	if (color < 0 &&
		material < best_bl
	) {
		best_bl = material;
		better = true;
	}

#line 546 "chess.md"
;
	move(to, from);
	set(to, old);

#line 517 "chess.md"
;
		return better;
	}

#line 425 "chess.md"
;
	int best_move(
		int &from, int &to, int color,
		float &best_wh, float &best_bl
	) {
		
#line 465 "chess.md"

	for (int i = 0; i < 120; ++i) {
		if (board[i] != XX &&
			board[i] * color > 0
		) {
			
#line 528 "chess.md"

	moves(i, [&](int m) {
		if (eval_move(
			i, m, color,
			best_wh, best_bl
		)) {
			from = i; to = m;
		}
	});

#line 470 "chess.md"
;
		}
	}

#line 430 "chess.md"
;
		return color > 0 ?
			best_wh : best_bl;
	}

#line 440 "chess.md"

	#include <limits>
	using nlf =
		std::numeric_limits<float>;

#line 6 "chess.md"

	int main() {
		
#line 120 "chess.md"

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
		
#line 135 "chess.md"

	if (cmd == "p") {
		std::cout << "\n";
		
#line 147 "chess.md"

	for (int r = 7; r >= 0; --r) {
		std::cout << (r + 1) << " ";
		
#line 187 "chess.md"
;
	for (int f = 0; f <= 7; ++f) {
		std::cout << pieces_board[
			get(f, r)
		];
	}

#line 150 "chess.md"
;
		std::cout << "\n";
	}
	std::cout << "  abcdefgh\n\n";

#line 212 "chess.md"

	std::cout << "  material: " <<
		abs_material << " (" <<
		material << ")\n";

#line 371 "chess.md"

	std::cout << "  " <<
		(state.color > 0 ? "white" :
			"black")  << " is moving\n";

#line 138 "chess.md"
;
		std::cout << "\n";
		continue;
	}

#line 284 "chess.md"
 {
	const char *cs { cmd.c_str() };
	if (cmd.size() == 4 &&
		is_pos(cs) && is_pos(cs + 2)
	) {
		
#line 341 "chess.md"

	int from { pos_to_idx(cs) };
	int to { pos_to_idx(cs + 2) };
	move(from, to);

#line 289 "chess.md"
;
		continue;
	}
} 
#line 383 "chess.md"

	if (cmd == "m") {
		int from { 0 }, to { 0 };
		
#line 449 "chess.md"

	constexpr float inf {
		nlf::infinity()
	};
	float best_w { -1 * inf };
	float best_b { inf };
	best_move(
		from, to, state.color,
		best_w, best_b
	);

#line 386 "chess.md"
;
		if (from && to) {
			move(from, to);
			
#line 413 "chess.md"

	char pos[3];
	idx_to_pos(pos, from);
	std::cout << pos;
	idx_to_pos(pos, to);
	std::cout << pos << "\n";

#line 389 "chess.md"
;
		} else {
			std::cout << "no move\n";
		}
		continue;
	}

#line 818 "chess.md"

	if (cmd == "c") {
		
#line 828 "chess.md"

	for (int r = 0; r <= 7; ++r) {
		for (int f = 0; f <= 7; ++f) {
			board[f_r_to_idx(f, r)] = 0;
		}
	}
	material = abs_material = 0.0f;

#line 820 "chess.md"
;
		continue;
	}

#line 846 "chess.md"

	if (cmd == "w" || cmd == "b") {
		int color { cmd == "w" ? 1 : -1 };
		for (;;) {
			std::string piece;
			std::cin >> piece;
			if (piece == ".") { break; }
			
#line 874 "chess.md"

	const char *cs { piece.c_str() };
	if (piece.size() == 3 &&
		is_pos(cs + 1)
	) {
		
#line 888 "chess.md"

	int p { pos_to_idx(cs + 1) };
	switch (cs[0]) {
		case '.': set(p, 0); break;
		
#line 902 "chess.md"

	case 'P': set(p, color * WP); break;
	case 'R': set(p, color * WR); break;
	case 'B': set(p, color * WB); break;
	case 'N': set(p, color * WN); break;
	case 'Q': set(p, color * WQ); break;
	case 'K': set(p, color * WK); break;

#line 892 "chess.md"
;
		default:
			
#line 866 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 894 "chess.md"
;
	}
	add(board[p]);

#line 879 "chess.md"
;
		continue;
	}

#line 914 "chess.md"

	if (piece.size() == 2 && is_pos(cs)) {
		int p { pos_to_idx(cs) };
		set(p, color * WP);
		continue;
	}

#line 853 "chess.md"
;
			
#line 866 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 854 "chess.md"
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
