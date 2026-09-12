#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QVector>
#include <QTimer>
#include <QKeyEvent>
#include <QStack>
#include <QMessageBox>

struct GameState
{
    QVector<QVector<int>> boardState;
    int score;
    bool isGameOver;
};


enum class GameMode {
    Normal,
    Unlimited,
    Hard
};

class GameBoard : public QWidget 
{
    Q_OBJECT
    
public:
    static constexpr int N = 4;
    static constexpr int M = 4;
    static constexpr int K = 2048;
    static constexpr int P = 90;
    static constexpr int Q = 10;

    explicit GameBoard(QWidget *parent = nullptr);

    void startGame(GameMode mode, int rows = N, int cols = M);
    GameMode getMode() const { return currentMode; }

    void undo();

    void setGameMode(GameMode mode);

signals:
    void scoreUpdate(int currentScore);
    void bestScoreUpdate(int bestScore);
    void modeChange(GameMode newMode);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void hardModeTimeout();

private: 
    // Setup
    void setUpGrid(int rows, int cols);
    void clearGrid();
    void addRandomTile();

    GameMode currentMode;
    int rowCount;
    int colCount;

    QGridLayout *gridLayout;
    QVector<QVector<QLabel*>> tiles;

    // Game mechanics
    bool slideUp();
    bool slideDown();
    bool slideRight();
    bool slideLeft();

    bool mergeLine(QVector<int>& line);

    int getTileValue(int row, int col);

    // Undo stack
    QStack<GameState> undoStack;
    int currentScore;
    int bestScore;

    // Helper to capture current state
    GameState captureCurrentState();

    // Ending checks
    bool isEnded;

    bool checkWin();
    bool checkLose();
    void triggerLoseGame(const QString &title, const QString &message);
    void triggerWinGame();

    // UI Timer
    QLabel *timerLabel;
    QTimer *uiTimer;

    void updateTimerUI();
    
    QTimer *hardModeTimer;
};