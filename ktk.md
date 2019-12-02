# König und Turm gegen König

```
@Def(file: ktk.cpp)
	#include <iostream>

	struct Piece {
		int file;
		int rank;
	};

	std::ostream &operator<<(std::ostream &out, const Piece &p) {
		return out << (char) (p.file + 'a') << (char) (p.rank + '1');
	}

	std::istream &operator>>(std::istream &in, Piece &p) {
		std::string s;
		while (s.size() != 2 || s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[1] > '8') {
			in >> s;
		}
		p.file = s[0] - 'a';
		p.rank = s[1] - '1';
		return in;
	}

	int main() {
		Piece white_king, white_rook, black_king;
		std::cout << "Turmendspiel nach Torres y Quevedo\n\n";
		for (;;) {
			std::cout << "Eingabe der Ausgangsstellung\n";
			std::cout << "weißer König:    "; std::cin >> white_king;
			std::cout << "weißer Turm:     "; std::cin >> white_rook;
			std::cout << "schwarzer König: "; std::cin >> black_king;
			if (white_rook.file > 2 && white_rook.file < 5) {
				std::cout << "Turm nicht in einer Zone\n";
				continue;
			}
			if (black_king.rank >= white_rook.rank) {
				std::cout << "schwarzer König oberhalb des Turms\n";
				continue;
			}
			if (white_rook.rank >= white_king.rank) {
				std::cout << "Turm oberhalb des weißen Königs\n";
				continue;
			}
			break;
		}

		for (;;) {
			bool king_moved { false };
			if (black_king.file <= 2 && white_rook.file <= 2) {
				white_rook.file = 7;
			} else if (black_king.file >= 5 && white_rook.file >= 5) {
				white_rook.file = 0;
			} else if (white_rook.rank - black_king.rank > 1) {
				--white_rook.rank;
			} else if (white_king.rank - black_king.rank > 2) {
				--white_king.rank;
				king_moved = true;
			} else {
				int diff = black_king.file - white_king.file;
				if (diff == 0) {
					--white_rook.rank;
				} else if (diff % 2 == 0) {
					white_king.file += diff < 0 ? -1 : 1;
					king_moved = true;
				} else {
					switch (white_rook.file) {
						case 1:
						case 2:
						case 3:
						case 7:
							--white_rook.file;
							break;
						default:
							++white_rook.file;
					}
				}
			}
			if (king_moved) {
				std::cout << "ziehe König: " << white_king << "\n";
			} else {
				std::cout << "ziehe Turm: " << white_rook << "\n";
				if (white_rook.rank == 0) {
					std::cout << "MATT\n";
					break;
				}
			}
			std::cout << "Ihr Zug: "; std::cin >> black_king;
		}
	}
@End(file: ktk.cpp)
```
