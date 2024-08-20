#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include <iomanip>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FONT_SIZE = 24;
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 50;
const int TABLE_PADDING = 10; // Padding around table
const int TABLE_ROW_HEIGHT = FONT_SIZE + 10; // Height of each row
const int TABLE_HEADER_HEIGHT = FONT_SIZE + 20; // Height of the table header

enum ButtonState {
    BUTTON_IDLE,
    BUTTON_HOVERED,
    BUTTON_PRESSED
};

struct Button {
    SDL_Rect rect;
    SDL_Color idleColor;
    SDL_Color hoverColor;
    SDL_Color clickColor;
    string text;
    ButtonState state;

    Button(int x, int y, int w, int h, SDL_Color idle, SDL_Color hover, SDL_Color click, const string& t)
        : rect({x, y, w, h}), idleColor(idle), hoverColor(hover), clickColor(click), text(t), state(BUTTON_IDLE) {}
};

struct Pasien {
    string nama;
    int id;
    string poli;
};

class AntrianRumahSakit {
private:
    queue<Pasien> antrian;
    Pasien pasienDilayani;
    bool melayani;
public:
    AntrianRumahSakit() : melayani(false) {}
    void daftarPasien(const string& nama, int id, const string& poli);
    void layaniPasien();
    void tampilkanAntrian(SDL_Renderer* renderer, TTF_Font* font, int startY, int& tableHeight);
    const queue<Pasien>& getAntrian() const { return antrian; }
    const Pasien& getPasienDilayani() const { return pasienDilayani; }
    bool isMelayani() const { return melayani; }
};

void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color color, int x, int y);
void renderButton(SDL_Renderer* renderer, TTF_Font* font, const Button& button);
void renderTable(SDL_Renderer* renderer, TTF_Font* font, const queue<Pasien>& antrian, int startY, int& tableHeight);
bool init(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font);
void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);

void AntrianRumahSakit::daftarPasien(const string& nama, int id, const string& poli) {
    Pasien pasienBaru = {nama, id, poli};
    antrian.push(pasienBaru);
}

void AntrianRumahSakit::layaniPasien() {
    if (!antrian.empty()) {
        pasienDilayani = antrian.front();
        antrian.pop();
        melayani = true;
    } else {
        melayani = false;
    }
}

void AntrianRumahSakit::tampilkanAntrian(SDL_Renderer* renderer, TTF_Font* font, int startY, int& tableHeight) {
    renderTable(renderer, font, antrian, startY, tableHeight);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color color, int x, int y) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void renderButton(SDL_Renderer* renderer, TTF_Font* font, const Button& button) {
    SDL_Color currentColor;
    switch (button.state) {
        case BUTTON_IDLE:
            currentColor = button.idleColor;
            break;
        case BUTTON_HOVERED:
            currentColor = button.hoverColor;
            break;
        case BUTTON_PRESSED:
            currentColor = button.clickColor;
            break;
    }

    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &button.rect);

    SDL_Color textColor = {255, 255, 255, 255}; // White color
    renderText(renderer, font, button.text, textColor, button.rect.x + TABLE_PADDING, button.rect.y + (button.rect.h - FONT_SIZE) / 2);
}

void renderHeader(SDL_Renderer* renderer, TTF_Font* font, int startX, int startY) {
    SDL_Color textColor = {0, 0, 0, 255}; // Black color
    int headerHeight = BUTTON_HEIGHT; // Same height as buttons

    string headerText = "ID   Nama   Poli";
    renderText(renderer, font, headerText, textColor, startX + 10, startY + (headerHeight / 2) - 10);
}

void renderTable(SDL_Renderer* renderer, TTF_Font* font, const queue<Pasien>& antrian, int startY, int& tableHeight) {
    int y = startY;
    int rows = antrian.size();
    tableHeight = TABLE_HEADER_HEIGHT + rows * (BUTTON_HEIGHT + TABLE_PADDING);

    SDL_Color textColor = {0, 0, 0, 255}; // Black color
    renderText(renderer, font, "ID", textColor, 50, y);
    renderText(renderer, font, "Nama", textColor, 150, y);
    renderText(renderer, font, "Poli", textColor, 400, y);
    y += TABLE_HEADER_HEIGHT + TABLE_PADDING; // Move down for the data rows

    queue<Pasien> tempQueue = antrian; // Create a copy of the queue to iterate
    while (!tempQueue.empty()) {
        const auto& pasien = tempQueue.front();

        renderText(renderer, font, to_string(pasien.id), textColor, 49, y); // ID column
        renderText(renderer, font, pasien.nama, textColor, 150, y);         // Nama column
        renderText(renderer, font, pasien.poli, textColor, 400, y);         // Poli column

        y += TABLE_ROW_HEIGHT + TABLE_PADDING; // Move down for the next row
        tempQueue.pop();
    }
}



bool init(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    *window = SDL_CreateWindow("Sistem Antrian Rumah Sakit", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == nullptr) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == nullptr) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << endl;
        return false;
    }

    *font = TTF_OpenFont("arial.TTF", FONT_SIZE); // Use arial.TTF
    if (*font == nullptr) {
        cout << "Failed to load font! TTF_Error: " << TTF_GetError() << endl;
        return false;
    }

    return true;
}

void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    TTF_CloseFont(font);
    font = nullptr;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    if (!init(&window, &renderer, &font)) {
        cout << "Failed to initialize!" << endl;
        return -1;
    }

    AntrianRumahSakit antrianRS;

    Button daftarButton(50, 50, BUTTON_WIDTH, BUTTON_HEIGHT, {0, 128, 0, 255}, {0, 255, 0, 255}, {0, 64, 0, 255}, "Daftar Pasien");
    Button layaniButton(50, 150, BUTTON_WIDTH, BUTTON_HEIGHT, {0, 0, 128, 255}, {0, 0, 255, 255}, {0, 0, 64, 255}, "Layani Pasien");
    Button keluarButton(50, 250, BUTTON_WIDTH, BUTTON_HEIGHT, {128, 0, 0, 255}, {255, 0, 0, 255}, {64, 0, 0, 255}, "Keluar");

    bool quit = false;
    bool inputMode = false;
    bool waitingForNama = false;
    bool waitingForId = false;
    bool waitingForPoli = false;
    string inputNama, inputIdStr, inputPoli;
    int inputId = 0;
    Button* activeButton = nullptr;

    SDL_StartTextInput();

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                SDL_Point mousePoint = {x, y};

                // Check if any button is clicked
                if (SDL_PointInRect(&mousePoint, &daftarButton.rect)) {
                    activeButton = &daftarButton;
                    inputMode = true;
                    waitingForNama = true; // Start by inputting Nama
                } else if (SDL_PointInRect(&mousePoint, &layaniButton.rect)) {
                    activeButton = &layaniButton;
                    if (!antrianRS.getAntrian().empty()) {
                        antrianRS.layaniPasien();
                    }
                } else if (SDL_PointInRect(&mousePoint, &keluarButton.rect)) {
                    quit = true;
                }
            } else if (e.type == SDL_TEXTINPUT) {
                if (activeButton == &daftarButton) {
                    if (waitingForNama) {
                        inputNama += e.text.text;
                    } else if (waitingForPoli) {
                        inputPoli += e.text.text;
                    } else if (waitingForId) {
                        inputIdStr += e.text.text;
                    }
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN && inputMode) {
                    if (activeButton == &daftarButton) {
                        if (waitingForNama) {
                            waitingForNama = false;
                            waitingForId = true;
                        } else if (waitingForId) {
                            inputId = stoi(inputIdStr); // Convert inputIdStr to integer for ID
                            waitingForId = false;
                            waitingForPoli = true;
                            inputIdStr.clear();
                        } else if (waitingForPoli) {
                            antrianRS.daftarPasien(inputNama, inputId, inputPoli);
                            inputNama.clear();
                            inputPoli.clear();
                            inputId = 0;
                            waitingForPoli = false;
                            inputMode = false;
                        }
                    }
                } else if (e.key.keysym.sym == SDLK_BACKSPACE && inputMode) {
                    if (waitingForNama) {
                        if (!inputNama.empty()) {
                            inputNama.pop_back();
                        }
                    } else if (waitingForPoli) {
                        if (!inputPoli.empty()) {
                            inputPoli.pop_back();
                        }
                    } else if (waitingForId) {
                        if (!inputIdStr.empty()) {
                            inputIdStr.pop_back();
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        renderButton(renderer, font, daftarButton);
        renderButton(renderer, font, layaniButton);
        renderButton(renderer, font, keluarButton);

        if (inputMode && activeButton == &daftarButton) {
            SDL_Color textColor = {0, 0, 0, 255}; // Black color
            string displayText;
            if (waitingForNama) {
                displayText = "Nama: " + inputNama;
            } else if (waitingForId) {
                displayText = "ID: " + inputIdStr;
            } else if (waitingForPoli) {
                displayText = "Poli: " + inputPoli;
            }
            renderText(renderer, font, displayText, textColor, 50, 350);
        }

        SDL_Color textColor = {0, 0, 0, 255}; // Black color
        int tableHeight;
        renderTable(renderer, font, antrianRS.getAntrian(), 400, tableHeight);

        int servingPatientY = 400 + tableHeight + TABLE_PADDING;

        if (antrianRS.isMelayani()) {
            const Pasien& pasien = antrianRS.getPasienDilayani();
            stringstream ss;
            ss << "Melayani pasien " << pasien.nama << " dengan ID: " << pasien.id << " untuk poli " << pasien.poli;
            renderText(renderer, font, ss.str(), textColor, 50, servingPatientY);
        } else if (antrianRS.getAntrian().empty() && !antrianRS.isMelayani()) {
            renderText(renderer, font, "Tidak ada pasien dalam antrian", textColor, 50, servingPatientY);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // 60 FPS
    }

    SDL_StopTextInput();
    close(window, renderer, font);
    return 0;
}