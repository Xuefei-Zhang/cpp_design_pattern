#include "memento_patterns.h"

#include <cassert>
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Memento Pattern Demo: State Preservation and Undo/Redo ===\n\n";

    std::cout << "1. Basic text editor with undo:\n";
    TextEditor editor;
    EditorHistory history;

    history.saveState(editor.save()); // Save initial empty state
    editor.write("Hello");
    history.saveState(editor.save());
    std::cout << "   After writing 'Hello': " << editor.getContent() << "\n";

    editor.write(" World");
    history.saveState(editor.save());
    std::cout << "   After writing ' World': " << editor.getContent() << "\n";

    editor.write("!");
    history.saveState(editor.save());
    std::cout << "   After writing '!': " << editor.getContent() << "\n";

    assert(editor.getContent() == "Hello World!");
    std::cout << "   [OK] Text written correctly\n\n";

    std::cout << "2. Undo operations:\n";
    auto memento1 = history.undo();
    if (memento1) {
        editor.restore(memento1);
    }
    std::cout << "   After first undo: " << editor.getContent() << "\n";
    assert(editor.getContent() == "Hello World");

    auto memento2 = history.undo();
    if (memento2) {
        editor.restore(memento2);
    }
    std::cout << "   After second undo: " << editor.getContent() << "\n";
    assert(editor.getContent() == "Hello");
    std::cout << "   [OK] Undo operations work correctly\n\n";

    std::cout << "3. Redo operations:\n";
    auto redoMemento1 = history.redo();
    if (redoMemento1) {
        editor.restore(redoMemento1);
    }
    std::cout << "   After first redo: " << editor.getContent() << "\n";
    assert(editor.getContent() == "Hello World");

    auto redoMemento2 = history.redo();
    if (redoMemento2) {
        editor.restore(redoMemento2);
    }
    std::cout << "   After second redo: " << editor.getContent() << "\n";
    assert(editor.getContent() == "Hello World!");
    std::cout << "   [OK] Redo operations work correctly\n\n";

    std::cout << "4. Undo/Redo state management:\n";
    std::cout << "   Can undo: " << (history.canUndo() ? "yes" : "no") << "\n";
    std::cout << "   Can redo: " << (history.canRedo() ? "yes" : "no") << "\n";
    assert(history.canUndo() == true); // We can still undo from current state
    assert(history.canRedo() == false); // No more redos available (at the end)
    std::cout << "   [OK] Undo/Redo state tracking works\n\n";

    std::cout << "5. New edit clears redo stack:\n";
    editor.clear();
    history.clear();

    editor.write("First");
    history.saveState(editor.save());
    editor.write(" Second");
    history.saveState(editor.save());

    history.undo(); // Now at "First"
    assert(history.canRedo());

    editor.write(" Third"); // This should clear redo stack
    history.saveState(editor.save());

    std::cout << "   Content after new edit: " << editor.getContent() << "\n";
    assert(history.canRedo() == false);
    std::cout << "   [OK] New edit clears redo history\n\n";

    std::cout << "6. Game state save and load:\n";
    Game game("Player1");
    GameSaveManager saveManager;

    std::cout << "   Initial state - Level: " << game.getLevel()
              << ", Score: " << game.getScore()
              << ", Health: " << game.getHealth() << "\n";

    game.play();
    game.addScore(500);
    saveManager.addSave("Save1", game.saveGame());
    std::cout << "   After playing - Level: " << game.getLevel()
              << ", Score: " << game.getScore()
              << ", Health: " << game.getHealth() << "\n";

    game.levelUp();
    game.play();
    game.takeDamage(30);
    saveManager.addSave("Save2", game.saveGame());
    std::cout << "   After level up - Level: " << game.getLevel()
              << ", Score: " << game.getScore()
              << ", Health: " << game.getHealth() << "\n";

    assert(game.getLevel() == 2);
    assert(game.getHealth() == 70);
    std::cout << "   [OK] Game progression tracked correctly\n\n";

    std::cout << "7. Load previous save:\n";
    auto save1 = saveManager.loadSave(0);
    game.loadGame(save1);
    std::cout << "   After loading Save1 - Level: " << game.getLevel()
              << ", Score: " << game.getScore()
              << ", Health: " << game.getHealth() << "\n";

    assert(game.getLevel() == 1);
    assert(game.getScore() == 600);
    assert(game.getHealth() == 100);
    std::cout << "   [OK] Game state restored from save\n\n";

    std::cout << "8. Multiple save slots:\n";
    std::cout << "   Total saves: " << saveManager.getSaveCount() << "\n";
    assert(saveManager.getSaveCount() == 2);

    for (std::size_t i = 0; i < saveManager.getSaveCount(); ++i) {
        auto save = saveManager.loadSave(i);
        std::cout << "   " << saveManager.getSaveName(i) << ": "
                  << save->getInfo() << "\n";
    }
    std::cout << "   [OK] Multiple save slots working\n\n";

    std::cout << "9. Canvas drawing with snapshots:\n";
    Canvas canvas;
    std::vector<std::shared_ptr<CanvasMemento>> snapshots;

    canvas.addShape("Circle");
    canvas.addShape("Square");
    snapshots.push_back(canvas.createSnapshot());
    std::cout << "   After adding 2 shapes: " << canvas.getShapeCount() << " shapes\n";

    canvas.addShape("Triangle");
    canvas.addShape("Rectangle");
    snapshots.push_back(canvas.createSnapshot());
    std::cout << "   After adding 2 more shapes: " << canvas.getShapeCount() << " shapes\n";

    assert(canvas.getShapeCount() == 4);
    std::cout << "   [OK] Canvas drawing tracked\n\n";

    std::cout << "10. Restore canvas to previous snapshot:\n";
    canvas.restoreSnapshot(snapshots[0]);
    std::cout << "   After restoring first snapshot: " << canvas.getShapeCount() << " shapes\n";
    assert(canvas.getShapeCount() == 2);

    const auto& shapes = canvas.getShapes();
    std::cout << "   Shapes on canvas: ";
    for (const auto& shape : shapes) {
        std::cout << shape << " ";
    }
    std::cout << "\n";
    assert(shapes[0] == "Circle");
    assert(shapes[1] == "Square");
    std::cout << "   [OK] Canvas restored to previous state\n\n";

    std::cout << "11. Cursor position tracking:\n";
    TextEditor editor2;
    editor2.write("Hello World");
    auto saveWithCursor = editor2.save();

    std::cout << "   Cursor position before: " << editor2.getCursorPosition() << "\n";
    editor2.setCursor(5);
    std::cout << "   Cursor position after manual set: " << editor2.getCursorPosition() << "\n";

    editor2.restore(saveWithCursor);
    std::cout << "   Cursor position after restore: " << editor2.getCursorPosition() << "\n";
    assert(editor2.getCursorPosition() == 11);
    std::cout << "   [OK] Cursor position restored correctly\n\n";

    std::cout << "Memento pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Memento Pattern:\n";
    std::cout << "- Preserves encapsulation boundaries\n";
    std::cout << "- Simplifies originator by delegating state management\n";
    std::cout << "- Enables undo/redo functionality\n";
    std::cout << "- Supports save/load and checkpointing\n";
    std::cout << "- Memento objects are immutable snapshots\n";

    return 0;
}
