#ifndef ASTER_H
#define ASTER_H

#include <iostream>
#include <vector>
#include <memory>

#define HEIGHT  (15)
#define WIDTH   (15)

enum Direction {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	COUNT
};

struct Points
{
	Points(int x, int y, Direction dir)
		: position{ x, y }
		, dir(dir)
	{
	}

	int position[2];
	Direction dir;
};

class Astar
{

	bool status;

	std::unique_ptr<int* []> pointer;

	const int h;
	const int w;

public:

    std::vector<Points> stopover;

	struct boola4_t {
		boola4_t(bool x = false, bool y = false, bool z = false, bool w = false)
			: stat{ x, y, z, w }
		{}

		bool stat[4];
	};

	Astar(int p[HEIGHT][WIDTH], int h, int w)
		: status(false)
		, pointer(new int* [h])
		, h(h)
		, w(w)
	{
		for (int i = 0; i < h; i++) {
			pointer[i] = p[i];
		}
	}

	~Astar() {

	}

	void searchPath(int startx, int starty, int goalx, int goaly) {

		//	方向を格納する
		Direction dir;
		//	現在の位置
		int nowposition[2]{ startx, starty };
		
		//	進んだ方向をマスクして戻らないようにする
		std::vector<boola4_t> stat_array;
		stat_array.emplace_back(false, false, false, false);

		//	進む方向に壁があるか保存する
		bool parition[4]{ false, false, false, false };
		//	距離
		int cost = HEIGHT + WIDTH;
		//	1つだけだと一旦遠くに行くときに膠着するのでその時の最短の値を格納するようにするための変数
		int buf_far = cost;
		
		//	配列の数
		int num = 0;
		//	方向を代入した数、0だと袋小路に入っているということ
		int counter(0);

		while (!status)
		{
			//	距離をありえない数にして初期化
			buf_far = HEIGHT + WIDTH;

			//	ステータスを描画
			//printboolean(stat_array[stat_array.size() - 1].stat);

			//	壁があるかを確認する
			if (pointer[nowposition[1] - 1][nowposition[0]] == 1) {
				//	進んだ方向をマスクして、戻らないようにする
				if (!stat_array[num].stat[0]) {
					parition[0] = true;
				}
			}
			if (pointer[nowposition[1] + 1][nowposition[0]] == 1) {
				if (!stat_array[num].stat[2]) {
					parition[2] = true;
				}
			}
			if (pointer[nowposition[1]][nowposition[0] + 1] == 1) {
				if (!stat_array[num].stat[1]) {
					parition[1] = true;
				}
			}
			if (pointer[nowposition[1]][nowposition[0] - 1] == 1) {
				if (!stat_array[num].stat[3]) {
					parition[3] = true;
				}
			}

			//	進める時、その時一番小さくなる値を代入する。一旦遠くに行ってもかまわない
			if (parition[1]) {
				if (buf_far >= calcfar(nowposition[0] + 2, nowposition[1], goalx, goaly)) {
					dir = RIGHT;
				}
				counter++;
			}
			if (parition[3]) {
				if (buf_far >= calcfar(nowposition[0] - 2, nowposition[1], goalx, goaly)) {
					dir = LEFT;
				}
				counter++;
			}
			if (parition[0]) {
				if (buf_far >= calcfar(nowposition[0], nowposition[1] - 2, goalx, goaly)) {
					dir = UP;
				}
				counter++;
			}
			if (parition[2]) {
				if (buf_far >= calcfar(nowposition[0], nowposition[1] + 2, goalx, goaly)) {
					dir = DOWN;
				}
				counter++;
			}

			//	最終的な距離を代入する
			cost = buf_far;

			/*
			std::cout << "num : " << num << std::endl;
			std::cout << "counter : " << counter << std::endl;
			*/

			//	カウンターが0で袋小路に入っていた時、1つ戻る
			if (counter == 0) {
				stat_array.pop_back();
				if (!stopover.empty()) {
					nowposition[0] = stopover[stopover.size() - 1].position[0];
					nowposition[1] = stopover[stopover.size() - 1].position[1];
					//std::cout << stopover[stopover.size() - 1].position[0] << ", " << stopover[stopover.size() - 1].position[1] << std::endl;

					stopover.pop_back();
					num--;

					//	2つ増加するからとしているが、一旦戻るとき果たしてこれでいいのか？
					cost += 2;
				}
			}
			else {
				//	前の場所の自分が進んだところを true 、今の位置の進んだ方向の反対側を true にすることで進んだ方向に戻ることを防ぐ
				stat_array.emplace_back(false, false, false, false);
				stopover.emplace_back(nowposition[0], nowposition[1], dir);
				num++;
				switch (dir) {
				case UP:
					//	ここでマスクをして戻らないようにしている
					stat_array[num - 1].stat[0] = true;
					stat_array[num].stat[2] = true;

					//	距離を再計算して代入
					cost = calcfar(nowposition[0], nowposition[1] - 2, goalx, goaly);
					nowposition[0] = nowposition[0];
					nowposition[1] = nowposition[1] - 2;
					break;
				case DOWN:
					stat_array[num - 1].stat[2] = true;
					stat_array[num].stat[0] = true;

					cost = calcfar(nowposition[0], nowposition[1] + 2, goalx, goaly);
					nowposition[0] = nowposition[0];
					nowposition[1] = nowposition[1] + 2;
					break;
				case RIGHT:
					stat_array[num - 1].stat[1] = true;
					stat_array[num].stat[3] = true;

					cost = calcfar(nowposition[0] + 2, nowposition[1], goalx, goaly);
					nowposition[0] = nowposition[0] + 2;
					nowposition[1] = nowposition[1];
					break;
				case LEFT:
					stat_array[num - 1].stat[3] = true;
					stat_array[num].stat[1] = true;

					cost = calcfar(nowposition[0] - 2, nowposition[1], goalx, goaly);
					nowposition[0] = nowposition[0] - 2;
					nowposition[1] = nowposition[1];
					break;
				default:
					break;
				}
			}

			//	四方の壁の有無を格納するブール型を初期化
			parition[0] = false;
			parition[1] = false;
			parition[2] = false;
			parition[3] = false;
			//	カウンターを初期化
			counter = 0;
			//	現在の位置とゴールが等しければ終了
			if (nowposition[0] == goalx && nowposition[1] == goaly) status = true;

			//std::cout << std::endl;
		}

		writePath(startx, starty, goalx, goaly);
	}

	void writePath(int startx, int starty, int goalx, int goaly) {

		pointer[starty][startx] = 2;
		
		for (Points a : stopover) {
			switch (a.dir) {
			case UP:
				pointer[a.position[1] - 1][a.position[0]] = 2;
				pointer[a.position[1] - 2][a.position[0]] = 2;
				break;
			case DOWN:
				pointer[a.position[1] + 1][a.position[0]] = 2;
				pointer[a.position[1] + 2][a.position[0]] = 2;
				break;
			case RIGHT:
				pointer[a.position[1]][a.position[0] + 1] = 2;
				pointer[a.position[1]][a.position[0] + 2] = 2;
				break;
			case LEFT:
				pointer[a.position[1]][a.position[0] - 1] = 2;
				pointer[a.position[1]][a.position[0] - 2] = 2;
				break;
			default:
				break;
			}
		}
	}

	void printboolean(bool obj[4]) {
		std::cout << "bool : ";
		for (int i = 0; i < 4; i++) {
			std::cout << static_cast<int>(obj[i]);
		}
		std::cout << std::endl;
	}

	int calcfar(int startx, int starty, int goalx, int goaly) const {
		return abs(goaly - starty) + abs(goalx - startx);
	}

	void printPoints() const
	{
		for (const auto a : stopover) {
			std::cout << "x : " << a.position[0] << ", y : " << a.position[1] << std::endl;
		}
	}

	std::vector<Points> getPath() const
	{
		return stopover;
	}
};

#endif // ASTER_H