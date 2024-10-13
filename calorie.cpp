#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ProccessSQL.hpp"
#include "raylib.h"

enum Screen { MAIN_MENU, ADD_DISH, VIEW_DISHES, GRAPH };
int selectedTextBox = -1;

void DrawTextCentered(const std::string& text, int posX, int posY, int fontSize,
                      Color color) {
  int textWidth = MeasureText(text.c_str(), fontSize);
  DrawText(text.c_str(), posX - textWidth / 2, posY, fontSize, color);
}

void HandleTextInput(std::string& text, int64_t textBoxIndex, float posX,
                     float posY, float width, float height) {
  Rectangle textbox = {posX, posY, width, height};

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      CheckCollisionPointRec(GetMousePosition(), textbox)) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    selectedTextBox = textBoxIndex;
  }

  if (selectedTextBox == textBoxIndex) {
    int64_t key = GetKeyPressed();
    if (key >= 32 && key <= 125 && text.size() < 13) {
      text += static_cast<char>(key);
    } else if (key == KEY_BACKSPACE && !text.empty()) {
      text.pop_back();
    }
  }
}

void ShowMainMenu(Screen& currentScreen, bool& dishAdded, ProccessSQL& db) {
  DrawTextCentered("Thor's Calorie Counter", 400, 20, 30, DARKBLUE);
  if (dishAdded) {
    DrawTextCentered("Dish added successfully!", 400, 100, 20, GREEN);
    dishAdded = false;
  }
  const int buttonOffsetFromBottom = 50;
  const int buttonHeight = 40;
  const int buttonSpacing = 10;
  int buttonY = 600 - buttonHeight - buttonOffsetFromBottom;

  DrawTextCentered("Add Dish", 400, buttonY, 30, DARKGREEN);
  Rectangle addButtonRect = {
      300, static_cast<float>(buttonY - buttonHeight / 2), 200, 40};

  if (CheckCollisionPointRec(GetMousePosition(), addButtonRect)) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      currentScreen = ADD_DISH;
    }
  }
  buttonY -= (buttonHeight + buttonSpacing);
  DrawTextCentered("View Dishes", 400, buttonY, 30, DARKGREEN);
  Rectangle viewButtonRect = {
      300, static_cast<float>(buttonY - buttonHeight / 2), 200, 40};

  if (CheckCollisionPointRec(GetMousePosition(), viewButtonRect)) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      currentScreen = VIEW_DISHES;
    }
  }
  buttonY -= (buttonHeight + buttonSpacing);
  DrawTextCentered("View Graph", 400, buttonY, 30, DARKGREEN);
  Rectangle graphButtonRect = {
      300, static_cast<float>(buttonY - buttonHeight / 2), 200, 40};

  if (CheckCollisionPointRec(GetMousePosition(), graphButtonRect)) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      currentScreen = GRAPH;
    }
  }
}

bool IsValidNumber(const std::string& str) {
  return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
void ShowAddDishScreen(Screen& currentScreen, ProccessSQL& db,
                       bool& dishAdded) {
  static std::string dishNameInput = "";
  static std::string caloriesInput = "";
  static std::string proteinsInput = "";
  static std::string fatsInput = "";
  static std::string carbsInput = "";
  const int screenWidth = 800;
  const float inputBoxWidth = 400;
  const float inputBoxHeight = 40;

  DrawTextCentered("Add New Dish", 400, 20, 30, DARKBLUE);
  DrawText("Dish Name:", screenWidth / 2 - inputBoxWidth / 2, 100, 20,
           DARKGRAY);
  HandleTextInput(dishNameInput, 0, screenWidth / 2 - inputBoxWidth / 2, 120,
                  inputBoxWidth, inputBoxHeight);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 120, inputBoxWidth,
                inputBoxHeight, DARKPURPLE);
  DrawText(dishNameInput.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           120 + 10, 20, BLACK);

  DrawText("Calories (per 100g):", screenWidth / 2 - inputBoxWidth / 2, 180, 20,
           DARKGRAY);
  HandleTextInput(caloriesInput, 1, screenWidth / 2 - inputBoxWidth / 2, 200,
                  inputBoxWidth, inputBoxHeight);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 200, inputBoxWidth,
                inputBoxHeight, DARKPURPLE);
  DrawText(caloriesInput.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           200 + 10, 20, BLACK);

  DrawText("Proteins (per 100g):", screenWidth / 2 - inputBoxWidth / 2, 260, 20,
           DARKGRAY);
  HandleTextInput(proteinsInput, 2, screenWidth / 2 - inputBoxWidth / 2, 280,
                  inputBoxWidth, inputBoxHeight);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 280, inputBoxWidth,
                inputBoxHeight, DARKPURPLE);
  DrawText(proteinsInput.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           280 + 10, 20, BLACK);

  DrawText("Fats (per 100g):", screenWidth / 2 - inputBoxWidth / 2, 340, 20,
           DARKGRAY);
  HandleTextInput(fatsInput, 3, screenWidth / 2 - inputBoxWidth / 2, 360,
                  inputBoxWidth, inputBoxHeight);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 360, inputBoxWidth,
                inputBoxHeight, DARKPURPLE);
  DrawText(fatsInput.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5, 360 + 10,
           20, BLACK);

  DrawText("Carbs (per 100g):", screenWidth / 2 - inputBoxWidth / 2, 420, 20,
           DARKGRAY);
  HandleTextInput(carbsInput, 4, screenWidth / 2 - inputBoxWidth / 2, 440,
                  inputBoxWidth, inputBoxHeight);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 440, inputBoxWidth,
                inputBoxHeight, DARKPURPLE);
  DrawText(carbsInput.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           440 + 10, 20, BLACK);

  DrawTextCentered("Press ENTER to Add Dish", 400, 520, 20, DARKGREEN);

  DrawTextCentered("Back", 400, 560, 20, GOLD);
  Rectangle backButtonRect = {300, 550, 200, 40};

  if (CheckCollisionPointRec(GetMousePosition(), backButtonRect)) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      currentScreen = MAIN_MENU;
    }
  }

  if (IsKeyPressed(KEY_ENTER)) {
    if (!dishNameInput.empty() && IsValidNumber(caloriesInput) &&
        IsValidNumber(proteinsInput) && IsValidNumber(fatsInput) &&
        IsValidNumber(carbsInput)) {
      float cal = std::stof(caloriesInput);
      std::string ingredients = "Proteins: " + proteinsInput +
                                ", Fats: " + fatsInput +
                                ", Carbs: " + carbsInput;

      db.AddDish(dishNameInput, cal, ingredients);
      dishAdded = true;

      dishNameInput = caloriesInput = proteinsInput = fatsInput = carbsInput =
          "";
      currentScreen = MAIN_MENU;
    } else {
      DrawTextCentered("Please enter valid numeric values!", 400, 480, 20, RED);
    }
  }
}

void ShowViewDishesScreen(Screen& currentScreen, ProccessSQL& db) {
  std::vector<Dish> dishes = db.GetDishes();
  DrawTextCentered("Dishes", 400, 20, 30, DARKBLUE);

  static int selectedDishIndex = -1;
  std::vector<int> selectedDishes;
  int startY = 100;

  for (size_t i = 0; i < dishes.size(); ++i) {
    DrawTextCentered(dishes[i].name, 100, startY + static_cast<int>(i) * 60, 20,
                     DARKGRAY);

    std::string nutrientsText =
        "Calories: " + std::to_string(dishes[i].calories) +
        ", Ingredients: " + dishes[i].ingredients;
    DrawText(nutrientsText.c_str(), 120, startY + static_cast<int>(i) * 60 + 20,
             20, DARKGRAY);

    Rectangle dishRect = {100, startY + static_cast<float>(i) * 60 - 20, 200,
                          40};

    if (CheckCollisionPointRec(GetMousePosition(), dishRect)) {
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedDishIndex = static_cast<int>(i);
        if (std::find(selectedDishes.begin(), selectedDishes.end(),
                      selectedDishIndex) == selectedDishes.end()) {
          selectedDishes.push_back(selectedDishIndex);
        } else {
          selectedDishes.erase(
              std::remove(selectedDishes.begin(), selectedDishes.end(),
                          selectedDishIndex),
              selectedDishes.end());
        }
      }
    }
  }
  DrawTextCentered("Selected Dishes:", 400, 500, 20, DARKGRAY);
  static int total = 0;
  for (int idx : selectedDishes) {
    DrawText(dishes[idx].name.c_str(), 200, 520 + (idx * 20), 20, DARKGRAY);
    total += dishes[idx].calories;
  }
  DrawTextCentered("Total Calories: " + std::to_string(total), 400,
                   540 + (selectedDishes.size() * 20), 20, DARKGREEN);
  if (selectedDishIndex != -1) {
    Dish& selectedDish = dishes[selectedDishIndex];
    DrawTextCentered("Edit Selected Dish", 400, 300, 30, DARKBLUE);
    static std::string editName = selectedDish.name;
    static std::string editCalories = std::to_string(selectedDish.calories);
    static std::string editIngredients = selectedDish.ingredients;

    DrawText("Name:", 300, 340, 20, DARKGRAY);
    HandleTextInput(editName, 0, 300, 360, 200, 30);
    DrawRectangle(300, 360, 200, 30, DARKPURPLE);
    DrawText(editName.c_str(), 305, 360 + 5, 20, BLACK);

    DrawText("Calories:", 300, 400, 20, DARKGRAY);
    HandleTextInput(editCalories, 1, 300, 420, 200, 30);
    DrawRectangle(300, 420, 200, 30, DARKPURPLE);
    DrawText(editCalories.c_str(), 305, 420 + 5, 20, BLACK);

    DrawText("Ingredients:", 300, 460, 20, DARKGRAY);
    HandleTextInput(editIngredients, 2, 300, 480, 200, 30);
    DrawRectangle(300, 480, 200, 30, DARKPURPLE);
    DrawText(editIngredients.c_str(), 305, 480 + 5, 20, BLACK);
    if (IsKeyPressed(KEY_ENTER)) {
      if (IsValidNumber(editCalories)) {
        double calories = std::stod(editCalories);
        db.UpdateDish(selectedDish.id, editName, calories, editIngredients);
        dishes = db.GetDishes();
        selectedDishIndex = -1;
      }
    }
  }
  DrawTextCentered("Press DELETE to Remove Selected Dishes", 400, 720, 20, RED);
  if (IsKeyPressed(KEY_DELETE) && !selectedDishes.empty()) {
    for (int idx : selectedDishes) {
      db.DeleteDish(dishes[idx].id);
    }
    dishes = db.GetDishes();
    selectedDishes.clear();
  }

  DrawTextCentered("Back", 400, 760, 20, GOLD);
  Rectangle backButtonRect = {300, 550, 200, 40};

  if (CheckCollisionPointRec(GetMousePosition(), backButtonRect)) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      currentScreen = MAIN_MENU;
    }
  }
}

int main() {
  InitWindow(800, 800, "Похудение Торика");
  SetTargetFPS(60);

  ProccessSQL db;
  bool dishAdded = false;
  Screen currentScreen = MAIN_MENU;

  while (!WindowShouldClose()) {
    switch (currentScreen) {
      case MAIN_MENU:
        ShowMainMenu(currentScreen, dishAdded, db);
        break;
      case ADD_DISH:
        ShowAddDishScreen(currentScreen, db, dishAdded);
        break;
      case VIEW_DISHES:
        ShowViewDishesScreen(currentScreen, db);
        break;
      case GRAPH:
        // DrawCalorieGraph();
        break;
      default:
        break;
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
