# Chess

```
@Def(file: chess.cpp)
	#include <array>
	#include <iostream>
	#include <cmath>

	int main() {
		using Board = std::array<signed char, 120>;
		Board board {
			100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
			100,   2,   4,   3,   5,   6,   3,   4,   2, 100,
			100,   1,   1,   1,   1,   1,   1,   1,   1, 100,
			100,   0,   0,   0,   0,   0,   0,   0,   0, 100,
			100,   0,   0,   0,   0,   0,   0,   0,   0, 100,
			100,   0,   0,   0,   0,   0,   0,   0,   0, 100,
			100,   0,   0,   0,   0,   0,   0,   0,   0, 100,
			100,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 100,
			100,  -2,  -4,  -3,  -5,  -6,  -3,  -4,  -2, 100,
			100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100, 100, 100, 100
		};
		float _pieces_mat[] {
			-200.0f, -9.0f, -3.25f, -3.5f, -5.0f, -1.0f,
			0.0f, 1.0f, 5.0f, 3.5f, 3.25f, 9.0f, 200.0f
		};
		float *pieces_mat { _pieces_mat + 6 };
		std::string _pieces_board[] {
			"♚", "♛", "♞", "♝", "♜", "♟", " ",
			"♙", "♖", "♗", "♘", "♕", "♔"
		};
		std::string *pieces_board { _pieces_board + 6 };

		struct State {
			bool wh_o_o { true };
			bool wh_o_o_o { true };
			bool bl_o_o { true };
			bool bl_o_o_o { true };
			char e_p_field { 0 };
		} state;
		float material { 0.0f };
		float abs_material { 481.0f };

		for (;;) {
			std::cout << "?> ";
			std::string cmd;
			if (! (std::cin >> cmd)) { break; }
			if (cmd == "p") {
				std::cout << "\n";
				for (int r = 7; r >= 0; --r) {
					std::cout << (r + 1) << " ";
					for (int f = 0; f <= 7; ++f) {
						signed char c { board[r * 10 + 21 + f] };
						std::cout << pieces_board[c];
					}
					std::cout << "\n";
				}
				std::cout << "  abcdefgh\n\n";
				std::cout << "  Material: " << abs_material << " (" << material << ")\n\n";
				continue;
			}
			if (cmd == "c") {
				for (int r = 0; r <= 7; ++r) {
					for (int f = 0; f <= 7; ++f) {
						board[r * 10 + 21 + f] = 0;
					}
				}
				material = abs_material = 0.0f;
				continue;
			}
			if (cmd == "w" || cmd == "b") {
				int mul = cmd == "w" ? 1 : -1;
				for (;;) {
					std::string piece;
					std::cin >> piece;
					if (piece == ".") { break; }
					if (piece.size() == 3 && piece[1] >= 'a' && piece[1] <= 'h' && piece[2] >= '1' && piece[2] <= 'h') {
						int p = (piece[1] - 'a') + 10 * (piece[2] - '1') + 21;
						material -= pieces_mat[board[p]];
						abs_material -= fabs(pieces_mat[board[p]]);
						bool found = true;
						switch (piece[0]) {
							case '.': board[p] = 0; break;
							case 'B': board[p] = mul * 1; break;
							case 'T': board[p] = mul * 2; break;
							case 'L': board[p] = mul * 3; break;
							case 'S': board[p] = mul * 4; break;
							case 'D': board[p] = mul * 5; break;
							case 'K': board[p] = mul * 6; break;
							default:
								std::cout << "unkonwn piece " << piece << "\n";
						}
						material += pieces_mat[board[p]];
						abs_material += fabs(pieces_mat[board[p]]);
						continue;
					}
					std::cout << "unknown piece " << piece << "\n";
				}
				continue;
			}
			if (cmd.size() == 4 && cmd[0] >= 'a' && cmd[0] <= 'h' && cmd[1] >= '1' && cmd[1] <= '8' && cmd[2] >= 'a' && cmd[2] <= 'h' && cmd[3] >= '1' && cmd[3] <= '8') {
				int from = (cmd[0] - 'a') + 10 * (cmd[1] - '1') + 21;
				int to = (cmd[2] - 'a') + 10 * (cmd[3] - '1') + 21;
				material -= pieces_mat[board[to]];
				abs_material -= fabs(pieces_mat[board[to]]);
				board[to] = board[from]; board[from] = 0;
				continue;
			}
			std::cout << "unknown command " << cmd << "\n";
		}
	}
@End(file: chess.cpp)
```

