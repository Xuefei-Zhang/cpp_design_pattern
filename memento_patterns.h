#ifndef MEMENTO_PATTERNS_H
#define MEMENTO_PATTERNS_H

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

// Memento stores internal state of Originator
class TextMemento {
private:
    std::string state_;
    int cursorPosition_;

    friend class TextEditor;

    TextMemento(const std::string& state, int cursor)
        : state_(state), cursorPosition_(cursor) {}

public:
    // Memento is opaque to outside world
    std::string getPreview() const {
        if (state_.length() > 20) {
            return state_.substr(0, 17) + "...";
        }
        return state_;
    }
};

// Originator creates and restores from Mementos
class TextEditor {
private:
    std::string content_;
    int cursorPosition_;

public:
    TextEditor() : content_(""), cursorPosition_(0) {}

    void write(const std::string& text) {
        content_ += text;
        cursorPosition_ = static_cast<int>(content_.length());
    }

    void setText(const std::string& text) {
        content_ = text;
        cursorPosition_ = static_cast<int>(content_.length());
    }

    void setCursor(int position) {
        if (position >= 0 && position <= static_cast<int>(content_.length())) {
            cursorPosition_ = position;
        }
    }

    std::shared_ptr<TextMemento> save() const {
        return std::shared_ptr<TextMemento>(new TextMemento(content_, cursorPosition_));
    }

    void restore(const std::shared_ptr<TextMemento>& memento) {
        if (memento) {
            content_ = memento->state_;
            cursorPosition_ = memento->cursorPosition_;
        }
    }

    const std::string& getContent() const {
        return content_;
    }

    int getCursorPosition() const {
        return cursorPosition_;
    }

    void clear() {
        content_.clear();
        cursorPosition_ = 0;
    }
};

// Caretaker manages mementos
class EditorHistory {
private:
    std::vector<std::shared_ptr<TextMemento>> history_;
    int currentIndex_;

public:
    EditorHistory() : currentIndex_(-1) {}

    void saveState(const std::shared_ptr<TextMemento>& memento) {
        // Remove all states after current index
        if (currentIndex_ < static_cast<int>(history_.size()) - 1) {
            history_.erase(history_.begin() + currentIndex_ + 1, history_.end());
        }

        history_.push_back(memento);
        ++currentIndex_;
    }

    std::shared_ptr<TextMemento> undo() {
        if (!canUndo()) {
            return nullptr;
        }

        --currentIndex_;
        return history_[currentIndex_];
    }

    std::shared_ptr<TextMemento> redo() {
        if (!canRedo()) {
            return nullptr;
        }

        ++currentIndex_;
        return history_[currentIndex_];
    }

    bool canUndo() const {
        return currentIndex_ > 0;
    }

    bool canRedo() const {
        return currentIndex_ < static_cast<int>(history_.size()) - 1;
    }

    std::size_t getUndoCount() const {
        return currentIndex_;
    }

    std::size_t getRedoCount() const {
        if (currentIndex_ < 0) return 0;
        return history_.size() - currentIndex_ - 1;
    }

    void clear() {
        history_.clear();
        currentIndex_ = -1;
    }
};

// Game State Example
class GameStateMemento {
private:
    int level_;
    int score_;
    int health_;
    std::string playerName_;

    friend class Game;

    GameStateMemento(int level, int score, int health, const std::string& name)
        : level_(level), score_(score), health_(health), playerName_(name) {}

public:
    std::string getInfo() const {
        return "Level:" + std::to_string(level_) + " Score:" + std::to_string(score_) +
               " Health:" + std::to_string(health_);
    }
};

class Game {
private:
    int level_;
    int score_;
    int health_;
    std::string playerName_;

public:
    Game(const std::string& name)
        : level_(1), score_(0), health_(100), playerName_(name) {}

    void play() {
        score_ += 100;
    }

    void takeDamage(int damage) {
        health_ -= damage;
        if (health_ < 0) {
            health_ = 0;
        }
    }

    void levelUp() {
        ++level_;
        health_ = 100; // Restore health on level up
    }

    void addScore(int points) {
        score_ += points;
    }

    std::shared_ptr<GameStateMemento> saveGame() const {
        return std::shared_ptr<GameStateMemento>(
            new GameStateMemento(level_, score_, health_, playerName_)
        );
    }

    void loadGame(const std::shared_ptr<GameStateMemento>& memento) {
        if (memento) {
            level_ = memento->level_;
            score_ = memento->score_;
            health_ = memento->health_;
            playerName_ = memento->playerName_;
        }
    }

    int getLevel() const { return level_; }
    int getScore() const { return score_; }
    int getHealth() const { return health_; }
    const std::string& getPlayerName() const { return playerName_; }
};

class GameSaveManager {
private:
    std::vector<std::shared_ptr<GameStateMemento>> saves_;
    std::vector<std::string> saveNames_;

public:
    void addSave(const std::string& name, const std::shared_ptr<GameStateMemento>& memento) {
        saveNames_.push_back(name);
        saves_.push_back(memento);
    }

    std::shared_ptr<GameStateMemento> loadSave(std::size_t index) const {
        if (index < saves_.size()) {
            return saves_[index];
        }
        return nullptr;
    }

    std::size_t getSaveCount() const {
        return saves_.size();
    }

    std::string getSaveName(std::size_t index) const {
        if (index < saveNames_.size()) {
            return saveNames_[index];
        }
        return "";
    }
};

// Canvas Drawing Example
class CanvasMemento {
private:
    std::vector<std::string> shapes_;

    friend class Canvas;

    explicit CanvasMemento(const std::vector<std::string>& shapes)
        : shapes_(shapes) {}

public:
    std::size_t getShapeCount() const {
        return shapes_.size();
    }
};

class Canvas {
private:
    std::vector<std::string> shapes_;

public:
    void addShape(const std::string& shape) {
        shapes_.push_back(shape);
    }

    void removeLastShape() {
        if (!shapes_.empty()) {
            shapes_.pop_back();
        }
    }

    std::shared_ptr<CanvasMemento> createSnapshot() const {
        return std::shared_ptr<CanvasMemento>(new CanvasMemento(shapes_));
    }

    void restoreSnapshot(const std::shared_ptr<CanvasMemento>& memento) {
        if (memento) {
            shapes_ = memento->shapes_;
        }
    }

    const std::vector<std::string>& getShapes() const {
        return shapes_;
    }

    std::size_t getShapeCount() const {
        return shapes_.size();
    }

    void clear() {
        shapes_.clear();
    }
};

// Inline usage examples:
// TextEditor editor;
// EditorHistory history;
// editor.write("Hello");
// history.saveState(editor.save());
// editor.write(" World");
// editor.restore(history.undo()); // Back to "Hello"

#endif // MEMENTO_PATTERNS_H
