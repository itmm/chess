
#line 5 "chess.md"

	
#line 16 "chess.md"

	#include <iostream>

#line 50 "chess.md"

	signed char board[120] {
		
#line 62 "chess.md"

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

#line 52 "chess.md"

	};

#line 124 "chess.md"

	inline int f_r_to_idx(
		int f, int r
	) {
		return r * 10 + f + 21;
	}

#line 138 "chess.md"

	inline signed char get(
		int f, int r
	) {
		return board[f_r_to_idx(f, r)];
	}

#line 165 "chess.md"

	float material { 0.0f };
	float abs_material { 481.0f };

#line 183 "chess.md"

	const float _pieces_mat[] {
		-200.0f, -9.0f, -3.25f, -3.5f,
		-5.0f, -1.0f, 0.0f, 1.0f, 5.0f,
		3.5f, 3.25f, 9.0f, 200.0f
	};
	const float *pieces_mat {
		_pieces_mat + 6
	};

#line 198 "chess.md"

	#include <cmath>

#line 205 "chess.md"

	void add(signed char f) {
		float m { pieces_mat[f] };
		material += m;
		abs_material += fabs(m);
	}

#line 216 "chess.md"

	void sub(signed char f) {
		float m { pieces_mat[f] };
		material -= m;
		abs_material -= fabs(m);
	}

#line 230 "chess.md"

	bool is_pos(const char *p) {
		if (p[0] < 'a' || p[0] > 'h') {
			return false;
		}
		if (p[1] < '1' || p[1] > '8') {
			return false;
		}
		return true;
	}

#line 259 "chess.md"

	int pos_to_idx(const char *p) {
		if (! is_pos(p)) { return 0; }
		int f { p[0] - 'a' };
		int r { p[1] - '1' };
		return f_r_to_idx(f, r);
	}

#line 6 "chess.md"

	int main() {
		
#line 83 "chess.md"

	const std::string _pieces_board[] {
		"♚", "♛", "♞", "♝", "♜",
		"♟", ".", "♙", "♖",
		"♗", "♘", "♕", "♔"
	};
	const std::string *pieces_board {
		_pieces_board + 6
	};

#line 391 "chess.md"

	struct State {
		bool whs_turn { true };
		bool wh_o_o { true };
		bool wh_o_o_o { true };
		bool bl_o_o { true };
		bool bl_o_o_o { true };
		int e_p_field { 0 };
	} state;

#line 8 "chess.md"
;
		
#line 23 "chess.md"

	for (;;) {
		std::cout << "?> ";
		std::string cmd;
		if (! (std::cin >> cmd)) {
			break;
		}
		
#line 98 "chess.md"

	if (cmd == "p") {
		std::cout << "\n";
		
#line 109 "chess.md"

	for (int r = 7; r >= 0; --r) {
		std::cout << (r + 1) << " ";
		
#line 149 "chess.md"
;
	for (int f = 0; f <= 7; ++f) {
		std::cout << pieces_board[
			get(f, r)
		];
	}

#line 112 "chess.md"
;
		std::cout << "\n";
	}
	std::cout << "  abcdefgh\n\n";

#line 174 "chess.md"

	std::cout << "  Material: " <<
		abs_material << " (" <<
		material << ")\n\n";

#line 101 "chess.md"
;
		continue;
	}

#line 246 "chess.md"
 {
	const char *cs { cmd.c_str() };
	if (cmd.size() == 4 &&
		is_pos(cs) && is_pos(cs + 2)
	) {
		
#line 272 "chess.md"

	int from { pos_to_idx(cs) };
	int to { pos_to_idx(cs + 2) };
	if (from && to) {
		sub(board[to]);
		board[to] = board[from];
		board[from] = 0;
	}

#line 251 "chess.md"
;
		continue;
	}
} 
#line 291 "chess.md"

	if (cmd == "c") {
		
#line 301 "chess.md"

	for (int r = 0; r <= 7; ++r) {
		for (int f = 0; f <= 7; ++f) {
			board[f_r_to_idx(f, r)] = 0;
		}
	}
	material = abs_material = 0.0f;

#line 293 "chess.md"
;
		continue;
	}

#line 319 "chess.md"

	if (cmd == "w" || cmd == "b") {
		int mul = cmd == "w" ? 1 : -1;
		for (;;) {
			std::string piece;
			std::cin >> piece;
			if (piece == ".") { break; }
			
#line 347 "chess.md"
 {
	const char *cs { piece.c_str() };
	if (piece.size() == 3 &&
		is_pos(cs + 1)
	) {
		
#line 361 "chess.md"

	int p { pos_to_idx(cs + 1) };
	sub(board[p]);
	switch (cs[0]) {
		case '.': board[p] = 0; break;
		
#line 376 "chess.md"

	case 'B': board[p] = mul * 1; break;
	case 'T': board[p] = mul * 2; break;
	case 'L': board[p] = mul * 3; break;
	case 'S': board[p] = mul * 4; break;
	case 'D': board[p] = mul * 5; break;
	case 'K': board[p] = mul * 6; break;

#line 366 "chess.md"
;
		default:
			
#line 339 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 368 "chess.md"
;
	}
	add(board[p]);

#line 352 "chess.md"
;
		continue;
	}
} 
#line 326 "chess.md"
;
			
#line 339 "chess.md"

	std::cerr << "unknown piece " <<
		piece << "\n";

#line 327 "chess.md"
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
