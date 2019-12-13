
#line 5 "chess.md"

	
#line 16 "chess.md"

	#include <iostream>

#line 50 "chess.md"

	using Elem = signed char;
	
#line 64 "chess.md"

	constexpr Elem XX { 100 };
	constexpr Elem X { XX };
	constexpr Elem EE { 0 };

#line 75 "chess.md"

	constexpr Elem WP { 1 };
	constexpr Elem WR { 2 };
	constexpr Elem WB { 3 };
	constexpr Elem WN { 4 };
	constexpr Elem WQ { 5 };
	constexpr Elem WK { 6 };

#line 87 "chess.md"

	constexpr Elem BP { -1 };
	constexpr Elem BR { -2 };
	constexpr Elem BB { -3 };
	constexpr Elem BN { -4 };
	constexpr Elem BQ { -5 };
	constexpr Elem BK { -6 };

#line 52 "chess.md"
;
	Elem board[120] {
		
#line 100 "chess.md"

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

#line 54 "chess.md"

	};

#line 163 "chess.md"

	inline int f_r_to_idx(
		int f, int r
	) {
		return r * 10 + f + 21;
	}

#line 177 "chess.md"

	inline Elem get(
		int f, int r
	) {
		return board[f_r_to_idx(f, r)];
	}

#line 204 "chess.md"

	float material { 0.0f };
	float abs_material { 481.0f };

#line 222 "chess.md"

	const float _pieces_mat[] {
		-200.0f, -9.0f, -3.25f, -3.5f,
		-5.0f, -1.0f, 0.0f, 1.0f, 5.0f,
		3.5f, 3.25f, 9.0f, 200.0f
	};
	const float *pieces_mat {
		_pieces_mat + 6
	};

#line 237 "chess.md"

	#include <cmath>

#line 244 "chess.md"

	void add(Elem e) {
		float m { pieces_mat[e] };
		material += m;
		abs_material += fabs(m);
	}

#line 255 "chess.md"

	void sub(Elem e) {
		float m { pieces_mat[e] };
		material -= m;
		abs_material -= fabs(m);
	}

#line 269 "chess.md"

	bool is_pos(const char *p) {
		if (p[0] < 'a' || p[0] > 'h') {
			return false;
		}
		if (p[1] < '1' || p[1] > '8') {
			return false;
		}
		return true;
	}

#line 298 "chess.md"

	int pos_to_idx(const char *p) {
		if (! is_pos(p)) { return 0; }
		int f { p[0] - 'a' };
		int r { p[1] - '1' };
		return f_r_to_idx(f, r);
	}

#line 311 "chess.md"

	inline void set(int pos, Elem e) {
		sub(board[pos]);
		board[pos] = e;
		add(board[pos]);
	}

#line 323 "chess.md"

	
#line 353 "chess.md"

	struct State {
		int color { 1 };
		
#line 1014 "chess.md"

	bool wh_o_o { true };
	bool wh_o_o_o { true };
	bool bl_o_o { true };
	bool bl_o_o_o { true };
	int e_p_field { 0 };

#line 356 "chess.md"
;
	} state;

#line 324 "chess.md"
;
	inline void move(
		int from, int to, int promo
	) {
		if (from && to) {
			set(to, promo ?: board[from]);
			set(from, 0);
			
#line 363 "chess.md"

	state.color = -state.color;

#line 331 "chess.md"
;
		}
	}

#line 370 "chess.md"

	inline bool is_wh(int c) {
		return c > 0;
	}

#line 379 "chess.md"

	inline bool is_bl(int c) {
		return c < 0;
	}

#line 420 "chess.md"

	void idx_to_pos(char *pos, int idx) {
		pos[0] = idx % 10 + 'a' - 1;
		pos[1] = idx / 10 + '1' - 2;
		pos[2] = '\0';
	}

#line 446 "chess.md"

	
#line 488 "chess.md"

	inline bool same_c(int p, int c) {
		return board[p] * c > 0;
	}

#line 497 "chess.md"

	inline bool opp_c(int p, int c) {
		return board[p] * c < 0;
	}

#line 536 "chess.md"

	
#line 521 "chess.md"

	
#line 650 "chess.md"

	bool is_pseudo_valid(
		int f, int t, int color
	) {
		auto x { board[t] };
		if (x == XX) { return false; }
		if (same_c(t, color)) {
			return false;
		}
		return true;
	}

#line 667 "chess.md"

	template<typename I>
	void add_if_valid(
		const I &it, int f, int t, int c
	) {
		if (is_pseudo_valid(f, t, c)) {
			it(t, 0);
		}
	}

#line 681 "chess.md"

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

#line 727 "chess.md"

	template<typename I>
	void add_pawn(
		const I &it, int f, int t, int c
	) {
		if (is_pseudo_valid(f, t, c)) {
			int r { t / 10 - 2 };
			if (r == 0 || r == 7) {
				it(t, WQ * c);
				it(t, WN * c);
				it(t, WB * c);
				it(t, WR * c);
			} else { 
				it(t, 0);
			}
		}
	}

#line 522 "chess.md"

	template<typename I>
	void pseudo_moves(
		int f, const I &it
	) {
		
#line 638 "chess.md"

	auto p { board[f] };
	if (p == EE || p == XX) { return; }
	int c { is_wh(p) ? 1 : -1 };
	
#line 699 "chess.md"

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

#line 715 "chess.md"

	if (p == WP) {
		
#line 749 "chess.md"

	if (board[f + 10] == 0) {
		add_pawn(it, f, f + 10, c);
	}
	if (opp_c(f + 9, c)) {
		add_pawn(it, f, f + 9, c);
	}
	if (opp_c(f + 11, c)) {
		add_pawn(it, f, f + 11, c);
	}

#line 765 "chess.md"

	int r { f / 10 - 2 };
	if (r == 1 && board[f + 10] == 0 &&
		board[f + 20] == 0
	) {
		add_if_valid(it, f, f + 20, c);
	}

#line 717 "chess.md"
;
	} else if (p == BP) {
		
#line 778 "chess.md"

	if (board[f - 10] == 0) {
		add_pawn(it, f, f - 10, c);
	}
	if (opp_c(f - 9, c)) {
		add_pawn(it, f, f - 9, c);
	}
	if (opp_c(f - 11, c)) {
		add_pawn(it, f, f - 11, c);
	}

#line 794 "chess.md"

	int r { f / 10 - 2 };
	if (r == 6 && board[f - 10] == 0 &&
		board[f - 20] == 0
	) {
		add_if_valid(it, f, f - 20, c);
	}

#line 719 "chess.md"
;
	}

#line 807 "chess.md"

	if (p == WR || p == BR) {
		add_row(it, f, c, -10);
		add_row(it, f, c, -1);
		add_row(it, f, c, 1);
		add_row(it, f, c, 10);
	}

#line 819 "chess.md"

	if (p == WB || p == BB) {
		add_row(it, f, c, -11);
		add_row(it, f, c, -9);
		add_row(it, f, c, 9);
		add_row(it, f, c, 11);
	}

#line 831 "chess.md"

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

#line 847 "chess.md"

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

#line 642 "chess.md"
;

#line 527 "chess.md"
;
	}

#line 537 "chess.md"

	template<typename I>
	void moves(int f, const I &it) {
		int c {
			is_wh(board[f]) ? 1 : -1
		};
		pseudo_moves(f, [&](
			int t, Elem e
		) {
			
#line 863 "chess.md"

	
#line 601 "chess.md"

	auto old_f { board[f] };
	auto old_t { board[t] };
	move(f, t, e);

#line 864 "chess.md"
;
	bool in_check { false };
	for (int i = 0; i < 120; ++i) {
		if (opp_c(i, c)) {
			
#line 879 "chess.md"

	pseudo_moves(i, [&](int m, Elem e) {
		if (board[m] == WK * c) {
			in_check = true;
		}
	});
	if (in_check) { break; }

#line 868 "chess.md"

		}
	}
	
#line 610 "chess.md"

	set(t, old_t);
	set(f, old_f);

#line 871 "chess.md"
;
	if (! in_check) { it(t, e); }

#line 546 "chess.md"
;
		});
	}

#line 556 "chess.md"

	bool eval_move(
		int f, int t, int c,
		float &best_wh, float &best_bl,
		Elem e
	) {
		bool better { false };
		
#line 590 "chess.md"

	
#line 601 "chess.md"

	auto old_f { board[f] };
	auto old_t { board[t] };
	move(f, t, e);

#line 591 "chess.md"
;
	
#line 618 "chess.md"

	if (is_wh(c) && material > best_wh) {
		best_wh = material;
		better = true;
	}

#line 628 "chess.md"

	if (is_bl(c) && material < best_bl) {
		best_bl = material;
		better = true;
	}

#line 592 "chess.md"
;
	
#line 610 "chess.md"

	set(t, old_t);
	set(f, old_f);

#line 593 "chess.md"
;

#line 563 "chess.md"
;
		return better;
	}

#line 447 "chess.md"
;
	int best_move(
		int &from, int &to, int color,
		float &best_wh, float &best_bl,
		Elem &promo
	) {
		
#line 506 "chess.md"

	for (int i = 0; i < 120; ++i) {
		if (board[i] != XX &&
			same_c(i, color)
		) {
			
#line 574 "chess.md"

	moves(i, [&](int m, Elem e) {
		if (eval_move(
			i, m, color,
			best_wh, best_bl, e
		)) {
			from = i; to = m;
			promo = e;
		}
	});

#line 511 "chess.md"
;
		}
	}

#line 453 "chess.md"
;
		return is_wh(color) ?
			best_wh : best_bl;
	}

#line 463 "chess.md"

	#include <limits>
	using nlf =
		std::numeric_limits<float>;

#line 6 "chess.md"

	int main() {
		
#line 121 "chess.md"

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
		
#line 136 "chess.md"

	if (cmd == "p") {
		std::cout << "\n";
		
#line 148 "chess.md"

	for (int r = 7; r >= 0; --r) {
		std::cout << (r + 1) << " ";
		
#line 188 "chess.md"
;
	for (int f = 0; f <= 7; ++f) {
		std::cout << pieces_board[
			get(f, r)
		];
	}

#line 151 "chess.md"
;
		std::cout << "\n";
	}
	std::cout << "  abcdefgh\n\n";

#line 213 "chess.md"

	std::cout << "  material: " <<
		abs_material << " (" <<
		material << ")\n";

#line 388 "chess.md"

	std::cout << "  " <<
		(is_wh(state.color) ? "white" :
			"black")  << " is moving\n";

#line 139 "chess.md"
;
		std::cout << "\n";
		continue;
	}

#line 285 "chess.md"
 {
	const char *cs { cmd.c_str() };
	if (cmd.size() == 4 &&
		is_pos(cs) && is_pos(cs + 2)
	) {
		
#line 340 "chess.md"

	int from { pos_to_idx(cs) };
	int to { pos_to_idx(cs + 2) };
	move(from, to, 0);

#line 290 "chess.md"
;
		continue;
	}
} 
#line 400 "chess.md"

	if (cmd == "m") {
		int from { 0 }, to { 0 };
		Elem promo { EE };
		
#line 472 "chess.md"

	constexpr float inf {
		nlf::infinity()
	};
	float best_w { -1 * inf };
	float best_b { inf };
	best_move(
		from, to, state.color,
		best_w, best_b, promo
	);

#line 404 "chess.md"
;
		if (from && to) {
			move(from, to, promo);
			
#line 431 "chess.md"

	char pos[3];
	idx_to_pos(pos, from);
	std::cout << pos;
	idx_to_pos(pos, to);
	std::cout << pos;
	if (promo) {
		std::cout << pieces_board[promo];
	}
	std::cout << "\n";

#line 407 "chess.md"
;
		} else {
			std::cout << "no move\n";
		}
		continue;
	}

#line 905 "chess.md"

	if (cmd == "c") {
		
#line 915 "chess.md"

	for (int r = 0; r <= 7; ++r) {
		for (int f = 0; f <= 7; ++f) {
			board[f_r_to_idx(f, r)] = 0;
		}
	}
	material = abs_material = 0.0f;

#line 907 "chess.md"
;
		continue;
	}

#line 933 "chess.md"

	if (cmd == "w" || cmd == "b") {
		int color { cmd == "w" ? 1 : -1 };
		for (;;) {
			std::string piece;
			std::cin >> piece;
			if (piece == ".") { break; }
			
#line 961 "chess.md"

	const char *cs { piece.c_str() };
	if (piece.size() == 3 &&
		is_pos(cs + 1)
	) {
		
#line 975 "chess.md"

	int p { pos_to_idx(cs + 1) };
	switch (cs[0]) {
		case '.': set(p, 0); break;
		
#line 989 "chess.md"

	case 'P': set(p, color * WP); break;
	case 'R': set(p, color * WR); break;
	case 'B': set(p, color * WB); break;
	case 'N': set(p, color * WN); break;
	case 'Q': set(p, color * WQ); break;
	case 'K': set(p, color * WK); break;

#line 979 "chess.md"
;
		default:
			
#line 953 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 981 "chess.md"
;
	}
	add(board[p]);

#line 966 "chess.md"
;
		continue;
	}

#line 1001 "chess.md"

	if (piece.size() == 2 && is_pos(cs)) {
		int p { pos_to_idx(cs) };
		set(p, color * WP);
		continue;
	}

#line 940 "chess.md"
;
			
#line 953 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 941 "chess.md"
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
