#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Option {
    string label;
    string key;
    int type; // 0: Bool, 1: Multi (Lang), 2: Multi (OS)
    int value;
};

void init_theme() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);  // bg
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // wind
    init_pair(3, COLOR_BLUE, COLOR_WHITE);  // text
}

void draw_frame(int win_h, int win_w) {
    int height, width;
    getmaxyx(stdscr, height, width);
    int start_y = (height - win_h) / 2;
    int start_x = (width - win_w) / 2;

    attron(COLOR_PAIR(2));
    for(int i=0; i<win_h; i++) mvhline(start_y + i, start_x, ' ', win_w);
    
    // shadow
    attron(COLOR_PAIR(1));
    mvvline(start_y + 1, start_x + win_w, ' ', win_h);
    mvhline(start_y + win_h, start_x + 1, ' ', win_w);
    
    attron(COLOR_PAIR(2));
    mvprintw(start_y, start_x + 2, " FC/(Fucking Calculator) Configuration ");
    mvprintw(start_y + win_h - 1, start_x + win_w - 20, " v0.7.5-stable ");
}

int main() {
    initscr(); noecho(); cbreak(); keypad(stdscr, TRUE); curs_set(0);
    init_theme();

    vector<Option> options = {
        {"POSIX mode", "CONFIG_POSIX", 0, 1},
        {"Without gcc lib", "CONFIG_NO_GCC_LIB", 0, 0},
        {"Optimize file", "CONFIG_OPTIMIZE", 0, 1},
        {"Language (0:EN, 1:JP, 2:ES)", "CONFIG_LANG", 1, 0},
        {"Target OS (0:Linux, 1:BSD, 2:Unix)", "CONFIG_TARGET", 2, 0}
    };

    string langs[] = {"English", "Japanese", "Spanish"};
    string targets[] = {"Linux", "FreeBSD", "OpenBSD", "NetBSD", "MacOS", "Solaris", "AIX", "HP-UX"};

    int highlight = 0;
    int btn_highlight = 0;

    while(true) {
        wbkgd(stdscr, COLOR_PAIR(1));
        int win_h = 20, win_w = 74;
        draw_frame(win_h, win_w);
        
        int start_y = (getmaxy(stdscr) - win_h) / 2;
        int start_x = (getmaxx(stdscr) - win_w) / 2;

        attron(COLOR_PAIR(2));
        mvprintw(start_y + 2, start_x + 4, "Main Menu -> Build Options");
        
        for(int i=0; i < options.size(); i++) {
            if(highlight == i) attron(A_REVERSE);
            
            if(options[i].type == 0) {
                mvprintw(start_y + 5 + i, start_x + 6, " [%c] %s", options[i].value ? '*' : ' ', options[i].label.c_str());
            } else if(options[i].type == 1) {
                mvprintw(start_y + 5 + i, start_x + 6, " < > %s: %s", options[i].label.c_str(), langs[options[i].value].c_str());
            } else {
                mvprintw(start_y + 5 + i, start_x + 6, " ( ) %s: %s", options[i].label.c_str(), targets[options[i].value].c_str());
            }
            
            attroff(A_REVERSE);
        }

        // Buttons
        string btns[] = {" Select ", " Help ", " Save ", " Exit "};
        for(int i=0; i<4; i++) {
            if(btn_highlight == i) attron(A_REVERSE);
            mvprintw(start_y + win_h - 3, start_x + 12 + (i * 12), "<%s>", btns[i].c_str());
            attroff(A_REVERSE);
        }

        int c = getch();
        switch(c) {
            case KEY_UP: highlight = (highlight > 0) ? highlight - 1 : options.size()-1; break;
            case KEY_DOWN: highlight = (highlight < options.size()-1) ? highlight + 1 : 0; break;
            case KEY_LEFT: btn_highlight = (btn_highlight > 0) ? btn_highlight - 1 : 3; break;
            case KEY_RIGHT: btn_highlight = (btn_highlight < 3) ? btn_highlight + 1 : 0; break;
            case ' ':
                if(options[highlight].type == 0) options[highlight].value = !options[highlight].value;
                else if(options[highlight].type == 1) options[highlight].value = (options[highlight].value + 1) % 3;
                else options[highlight].value = (options[highlight].value + 1) % 8;
                break;
            case 10: // ENTER
                if(btn_highlight == 2) { // Save
                    ofstream f(".config");
                    f << "# FC Config Generated for " << targets[options[4].value] << endl;
                    for(auto &o : options) f << o.key << "=" << o.value << endl;
                    f.close();
                    mvprintw(1, 1, "Configuration saved to .config!"); refresh(); getch();
                }
                if(btn_highlight == 3) { endwin(); return 0; }
                break;
        }
    }
}
