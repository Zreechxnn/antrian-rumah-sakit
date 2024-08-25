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

struct Patient {
    string name;
    int id;
    string department;
};

class HospitalQueue {
private:
    queue<Patient> queue;
    Patient currentPatient;
    bool serving;
public:
    HospitalQueue() : serving(false) {}
    void registerPatient(const string& name, int id, const string& department);
    void servePatient();
    void displayQueue(SDL_Renderer* renderer, TTF_Font* font, int startY, int& tableHeight);
    const queue<Patient>& getQueue() const { return queue; }
    const Patient& getCurrentPatient() const { return currentPatient; }
    bool isServing() const { return serving; }
};

void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color color, int x, int y);
void renderButton(SDL_Renderer* renderer, TTF_Font* font, const Button& button);
void renderTable(SDL_Renderer* renderer, TTF_Font* font, const queue<Patient>& queue, int startY, int& tableHeight);
bool init(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font);
void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);

void HospitalQueue::registerPatient(const string& name, int id, const string& department) {
    Patient newPatient = {name, id, department};
    queue.push(newPatient);
}

void HospitalQueue::servePatient() {
    if (!queue.empty()) {
        currentPatient = queue.front();
        queue.pop();
        serving = true;
    } else {
        serving = false;
    }
}

void HospitalQueue::displayQueue(SDL_Renderer* renderer, TTF_Font* font, int startY, int& tableHeight) {
    renderTable(renderer, font, queue, startY, tableHeight);
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

    string headerText = "ID   Name   Department";
    renderText(renderer, font, headerText, textColor, startX + 10, startY + (headerHeight / 2) - 10);
}

void renderTable(SDL_Renderer* renderer, TTF_Font* font, const queue<Patient>& queue, int startY, int& tableHeight) {
    int y = startY;
    int rows = queue.size();
    tableHeight = TABLE_HEADER_HEIGHT + rows * (BUTTON_HEIGHT + TABLE_PADDING);

    SDL_Color textColor = {0, 0, 0, 255}; // Black color
    renderText(renderer, font, "ID", textColor, 50, y);
    renderText(renderer, font, "Name", textColor, 150, y);
    renderText(renderer, font, "Department", textColor, 400, y);
    y += TABLE_HEADER_HEIGHT + TABLE_PADDING; // Move down for the data rows

    queue<Patient> tempQueue = queue; // Create a copy of the queue to iterate
    while (!tempQueue.empty()) {
        const auto& patient = tempQueue.front();

        renderText(renderer, font, to_string(patient.id), textColor, 49, y); // ID column
        renderText(renderer, font, patient.name, textColor, 150, y);         // Name column
        renderText(renderer, font, patient.department, textColor, 400, y);   // Department column

        y += TABLE_ROW_HEIGHT + TABLE_PADDING; // Move down for the next row
        tempQueue.pop();
    }
}

bool init(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    *window = SDL_CreateWindow("Hospital Queue System", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    HospitalQueue hospitalQueue;

    Button registerButton(50, 50, BUTTON_WIDTH, BUTTON_HEIGHT, {0, 128, 0, 255}, {0, 255, 0, 255}, {0, 64, 0, 255}, "Register Patient");
    Button serveButton(50, 150, BUTTON_WIDTH, BUTTON_HEIGHT, {0, 0, 128, 255}, {0, 0, 255, 255}, {0, 0, 64, 255}, "Serve Patient");
    Button exitButton(50, 250, BUTTON_WIDTH, BUTTON_HEIGHT, {128, 0, 0, 255}, {255, 0, 0, 255}, {64, 0, 0, 255}, "Exit");

    bool quit = false;
    bool inputMode = false;
    bool waitingForName = false;
    bool waitingForId = false;
    bool waitingForDepartment = false;
    string inputName, inputIdStr, inputDepartment;
    int inputId = 0;
    Button* activeButton = nullptr;

    SDL_StartTextInput();

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (SDL_PointInRect(&SDL_Point{mouseX, mouseY}, &registerButton.rect)) {
                    registerButton.state = BUTTON_PRESSED;
                    inputMode = true;
                    waitingForName = true;
                    waitingForId = false;
                    waitingForDepartment = false;
                } else if (SDL_PointInRect(&SDL_Point{mouseX, mouseY}, &serveButton.rect)) {
                    serveButton.state = BUTTON_PRESSED;
                    hospitalQueue.servePatient();
                } else if (SDL_PointInRect(&SDL_Point{mouseX, mouseY}, &exitButton.rect)) {
                    exitButton.state = BUTTON_PRESSED;
                    quit = true;
                } else {
                    registerButton.state = BUTTON_IDLE;
                    serveButton.state = BUTTON_IDLE;
                    exitButton.state = BUTTON_IDLE;
                }
            } else if (e.type == SDL_TEXTINPUT && inputMode) {
                if (waitingForName) {
                    inputName += e.text.text;
                } else if (waitingForId) {
                    inputIdStr += e.text.text;
                    if (isdigit(e.text.text[0])) {
                        inputId = stoi(inputIdStr);
                    }
                } else if (waitingForDepartment) {
                    inputDepartment += e.text.text;
                }
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                if (waitingForName) {
                    waitingForName = false;
                    waitingForId = true;
                } else if (waitingForId) {
                    waitingForId = false;
                    waitingForDepartment = true;
                } else if (waitingForDepartment) {
                    waitingForDepartment = false;
                    hospitalQueue.registerPatient(inputName, inputId, inputDepartment);
                    inputName.clear();
                    inputIdStr.clear();
                    inputDepartment.clear();
                    inputMode = false;
                }
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE) {
                if (waitingForName) {
                    if (!inputName.empty()) {
                        inputName.pop_back();
                    }
                } else if (waitingForId) {
                    if (!inputIdStr.empty()) {
                        inputIdStr.pop_back();
                    }
                } else if (waitingForDepartment) {
                    if (!inputDepartment.empty()) {
                        inputDepartment.pop_back();
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        renderButton(renderer, font, registerButton);
        renderButton(renderer, font, serveButton);
        renderButton(renderer, font, exitButton);

        int tableHeight;
        hospitalQueue.displayQueue(renderer, font, 350, tableHeight);

        if (hospitalQueue.isServing()) {
            renderText(renderer, font, "Serving patient " + hospitalQueue.getCurrentPatient().name + " with ID " + to_string(hospitalQueue.getCurrentPatient().id) + " in department " + hospitalQueue.getCurrentPatient().department, {0, 0, 0, 255}, 50, 500);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
    close(window, renderer, font);

    return 0;
}
