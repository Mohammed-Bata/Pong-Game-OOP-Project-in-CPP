#include<iostream>
#include<deque>
#include<conio.h>
#include<chrono>
#include<thread>


using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define z 122

const int height{ 30 };
const int width{ 50 };

class paddle {
public:
	paddle() {
		location.push_back((height / 2) + 2);
		location.push_back((height / 2) + 1);
		location.push_back(height / 2);
		location.push_back((height / 2) -1);
		location.push_back((height / 2) - 2);
	}

	void set_direction(int x) {
		cur_direction = x;
	}

	void set_paddle() {
		location.clear();
		location.push_back((height / 2) + 2);
		location.push_back((height / 2) + 1);
		location.push_back(height / 2);
		location.push_back((height / 2) - 1);
		location.push_back((height / 2) - 2);
	}

	void move_paddle() {
		switch (cur_direction) {
		case KEY_UP:
				location.push_front(location.front() - 1);
				location.pop_back();
			break;
		case KEY_DOWN:
				location.push_back(location.back() + 1);
				location.pop_front();
			break;
		}
	}
	bool isbody(int y) {
		deque<int>::iterator itr;
		itr = find(location.begin(), location.end(), y);
		if (itr != location.end()) {
			return true;
		}
		return false;
	}
	deque<int> location;
private:
	int cur_direction;
};

class computer_paddle:public paddle {
public:
	void move_cpaddle(int y) {
		if (location.front() < y) {
			location.pop_front();
			location.push_back(location.back() + 1);
		}
		if (location.front() >= y) {
			location.pop_back();
			location.push_front(location.front() - 1);
		}
	}
private:

};

class ball {
public:
	ball() {
		ball_location = { width / 2,height / 2 };
	}
	void move(paddle&player,computer_paddle&cm) {
		ball_location.first = ball_location.first + speed_x;
		ball_location.second = ball_location.second + speed_y;

		if ((ball_location.first >= height || ball_location.first<=0) && (ball_location.second > 0 && ball_location.second < width)) {
			speed_x *= -1;
		}
		if ((ball_location.second==width-2&&player.isbody(ball_location.first))||(ball_location.second == 2&& cm.isbody(ball_location.first))) {
			speed_y *= -1;
		}
	}

	pair<int,int> get_location() {
		return ball_location;
	}
	void set_location() {
		ball_location = { width / 2,height / 2 };
	}
private:
	int speed_x = 1;
	int speed_y = 1;
	pair<int, int>ball_location;
};

class result {
public:
	result() :player_score(0),computer_score(0) {}
	
	void player_goal() {
		player_score++;
	}
	void computer_goal() {
		computer_score++;
	}
	void draw() {
		cout <<"computer: "<< computer_score <<"                                 "
			<<"player: "<<player_score << endl;
	}
	pair<int, int> get_result() {
		return make_pair(player_score, computer_score);
	}
private:
	int player_score;
	int computer_score;
};

class game {
public:
	game() :isgameover(false) {};

	void draw_game(){
		system("cls");
		score.draw();
		for (int i = 0; i < height + 1; ++i) {
			for (int j = 0; j < width + 1; ++j) {
				if (i == 0 || j == 0 || j == width || i == height) {
					cout << "#";
				}
				else {
					if (bol.get_location() == make_pair(i, j)) {
						cout << "O";
					}
					else if ((j==width-1 && player.isbody(i))||(j == 1 && bot.isbody(i))) {
						cout << "|";
					}
					else {
						cout << " ";
					}
				}
			}
			cout << endl;
		}
	}

	void input() {
		if (_kbhit()) {
			int c = _getch();
			switch (c) {
			case KEY_UP:
				player.set_direction(KEY_UP);
				break;
			case KEY_DOWN:
				player.set_direction(KEY_DOWN);
				break;
			case z:
				isgameover = true;
				break;
			}
		}
	}

	void update_game() {
		bol.move(player,bot);
		player.move_paddle();
		bot.move_cpaddle(bol.get_location().first);
		if (bol.get_location().second <= 0&&!bot.isbody(bol.get_location().first)) {
			score.player_goal();
			bol.set_location();
			player.set_paddle();
		}
		else if (bol.get_location().second >= width&&!player.isbody(bol.get_location().first)) {
			score.computer_goal();
			bol.set_location();
			bot.set_paddle();
		}
		if (score.get_result().first >= 3 || score.get_result().second >= 3) {
			isgameover = true;
		}
	}

	bool isover()const {
		return isgameover;
	}

private:
	bool isgameover;
	paddle player;
	computer_paddle bot;
	result score;
	ball bol;
};

class user {
public:
	void play(game& gm) {
		while (!gm.isover()) {
			gm.draw_game();
			gm.input();
			gm.update_game();
			this_thread::sleep_for(std::chrono::milliseconds(400));
		}
		cout << endl << "game over";
	}
};

int main() {
	game sn;
	user us;
	us.play(sn);
}


