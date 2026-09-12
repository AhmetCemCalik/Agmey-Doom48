#include "gameboard.h"
#include "theme.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QPushButton>

GameBoard::GameBoard(QWidget *parent) : QWidget(parent), hardModeTimer(new QTimer(this))
{
    // Always listen for keyboard inputs
    setFocusPolicy(Qt::StrongFocus);

    // Layout for the entire screen
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget *boardContainer = new QWidget(this);
    boardContainer->setStyleSheet(Theme::getBoardContainerStyle());

    // Initialize the grid layout AND attach it to the boardContainer, not 'this'
    gridLayout = new QGridLayout(boardContainer);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(15, 15, 15, 15);

    // Add the finished board container to the center of the screen
    mainLayout->addWidget(boardContainer);

    // Set best score to zero
    bestScore = 0;


    // Set the timer
    timerLabel = new QLabel("5.00", this);
    timerLabel->setAlignment(Qt::AlignCenter);

    timerLabel->setStyleSheet(Theme::getTimerStyle());

    timerLabel->setFixedHeight(40);
    timerLabel->setFixedWidth(100);
    // Hide if not hard mode
    timerLabel->hide();

    mainLayout->addWidget(timerLabel, 0, Qt::AlignHCenter);

    uiTimer = new QTimer(this);

    connect(hardModeTimer, &QTimer::timeout, this, &GameBoard::hardModeTimeout);
    connect(uiTimer, &QTimer::timeout, this, &GameBoard::updateTimerUI);
}

void GameBoard::startGame(GameMode mode, int rows, int cols)
{
    undoStack.clear();
    isEnded = false;
    currentScore = 0;

    emit scoreUpdate(currentScore);

    currentMode = mode;
    setUpGrid(rows, cols);

    // Just in case K = 2
    if (checkWin()) {
        triggerWinGame();
    }
}

void GameBoard::setUpGrid(int rows, int cols)
{
    clearGrid();
    rowCount = rows;
    colCount = cols;

    tiles.resize(rows);

    QVector<QPair<int, int>> allSpots;

    for (int i = 0; i < rows; i++) {
        tiles[i].resize(cols);
        for (int j = 0; j < cols; j++) {

            QLabel *tile = new QLabel("", this);
            tile->setAlignment(Qt::AlignCenter);

            tile->setStyleSheet(Theme::getTileStyle(0));
            tile->setFixedSize(80, 80);

            gridLayout->addWidget(tile, i, j);
            tiles[i][j] = tile;

            // Add all the tiles to the list
            allSpots.append(qMakePair(i, j));
        }
    }

    // Select two tiles to initiate the game with two '2's
    int firstIndex = QRandomGenerator::global()->bounded(allSpots.size());
    QPair<int, int> spot1 = allSpots.takeAt(firstIndex);

    int secondIndex = QRandomGenerator::global()->bounded(allSpots.size());
    QPair<int, int> spot2 = allSpots.takeAt(secondIndex);

    tiles[spot1.first][spot1.second]->setText("2");
    tiles[spot1.first][spot1.second]->setStyleSheet(Theme::getTileStyle(2));

    tiles[spot2.first][spot2.second]->setText("2");
    tiles[spot2.first][spot2.second]->setStyleSheet(Theme::getTileStyle(2));
}

void GameBoard::clearGrid()
{
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if (tiles[i][j]) {
                gridLayout->removeWidget(tiles[i][j]);
                delete tiles[i][j];
            }
        }
    }
    tiles.clear();
}

void GameBoard::hardModeTimeout()
{
    // Implement forced move logic

    qDebug() << "5 Seconds passed!";

    GameState currentState = captureCurrentState();

    int slideDirection = (QRandomGenerator::global()->bounded(4));

    bool boardChanged = false;

    // Try all directions
    for (int i = 0; i < 4; ++i) {
        if (slideDirection == 1) {
            boardChanged = slideUp();
            qDebug() << "Forced Sliding UP";
            break;
        } else if (slideDirection == 2) {
            boardChanged = slideDown();
            qDebug() << "Forced Sliding DOWN";
            break;
        } else if (slideDirection == 3) {
            boardChanged = slideRight();
            qDebug() << "Forced Sliding RIGHT";
            break;
        } else {
            boardChanged = slideLeft();
            qDebug() << "Forced Sliding LEFT";
            break;
        }
        
        slideDirection++;
        slideDirection %= 4;
    }

    if (boardChanged) {
        undoStack.push(currentState);
        addRandomTile();

        // Check game end
        if (checkWin()) {
            triggerWinGame();
        } else if (checkLose()) {
            triggerLoseGame("Game Over!", "No valid moves left.");
        } 
        // Restart the timer if the game is still going
        else if (currentMode == GameMode::Hard) {
            hardModeTimer->start(5000);
        }
    }
}

void GameBoard::addRandomTile()
{
    QVector<QPair<int, int>> emptySpots;

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (tiles[i][j]->text() == "") {
                emptySpots.append(qMakePair(i, j));
            }
        }
    }

    if (emptySpots.isEmpty()) {
        return;
    }

    // Pick random tiles
    int randomIndex = QRandomGenerator::global()->bounded(emptySpots.size());
    QPair<int, int> spot = emptySpots[randomIndex];

    QLabel *tile = tiles[spot.first][spot.second];
    
    int tileValue = (QRandomGenerator::global()->bounded(100) < P) ? 2 : 4;
    tile->setText(QString::number(tileValue));

    tile->setStyleSheet(Theme::getTileStyle(tileValue));
}

int GameBoard::getTileValue(int row, int col)
{
    QString text = tiles[row][col]->text();
    return text.isEmpty() ? 0 : text.toInt();
}

void GameBoard::keyPressEvent(QKeyEvent *event)
{
    if (isEnded) {
        return;
    }
    // Capture game state
    GameState currentState = captureCurrentState();

    bool boardChanged = false;

    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        boardChanged = slideUp();
        qDebug() << "Sliding UP";
    }
    else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        boardChanged = slideDown();
        qDebug() << "Sliding DOWN";
    }
    else if (event->key() == Qt::Key_R || event->key() == Qt::Key_Right) {
        boardChanged = slideRight();
        qDebug() << "Sliding RIGHT";
    }
    else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        boardChanged = slideLeft();
        qDebug() << "Sliding LEFT";
    } else if (event->key() == Qt::Key_U) {
        undo();
        return;
    }
    else {
        QWidget::keyPressEvent(event);
        return;
    }

    if (boardChanged) {
        undoStack.push(currentState);
        addRandomTile();

        // Check game end
        if (checkWin()) {
            triggerWinGame();
        } else if (checkLose()) {
            triggerLoseGame("You Lose!", "No valid moves left.");
        } 
        // Restart the timer if the game is still going
        else if (currentMode == GameMode::Hard) {
            hardModeTimer->start(5000);
        }
    }
}

bool GameBoard::slideUp() 
{
    bool boardChanged = false;

    for (int i = 0; i < colCount; ++i) {
        
        // Get the line
        QVector<int> line;
        for (int j = 0; j < rowCount; ++j) {
            line.append(getTileValue(j, i));
        }

        bool rowChanged = mergeLine(line);
        if (rowChanged) {
            boardChanged = true;
            
            // Write back to the grid
            for (int j = 0; j < rowCount; ++j) {
                int val = line[j];
                tiles[j][i]->setText(val == 0 ? "" : QString::number(val));
                tiles[j][i]->setStyleSheet(Theme::getTileStyle(val));
            }
        }
    }
    return boardChanged;
}

bool GameBoard::slideDown() 
{
    bool boardChanged = false;

    for (int i = 0; i < colCount; ++i) {
        
        QVector<int> line;
        for (int j = rowCount - 1; j >= 0; --j) {
            line.append(getTileValue(j, i));
        }

        bool rowChanged = mergeLine(line);
        if (rowChanged) {
            boardChanged = true;
            
            for (int j = rowCount - 1; j >= 0; --j) {
                int val = line[rowCount - 1 - j];
                tiles[j][i]->setText(val == 0 ? "" : QString::number(val));
                tiles[j][i]->setStyleSheet(Theme::getTileStyle(val));
            }
        }
    }
    return boardChanged;
}

bool GameBoard::slideRight() 
{
    bool boardChanged = false;

    for (int i = 0; i < rowCount; ++i) {
        
        QVector<int> line;
        for (int j = colCount - 1; j >= 0; --j) {
            line.append(getTileValue(i, j));
        }

        bool rowChanged = mergeLine(line);
        if (rowChanged) {
            boardChanged = true;
            
            for (int j = colCount - 1; j >= 0; --j) {
                int val = line[colCount - j - 1];
                tiles[i][j]->setText(val == 0 ? "" : QString::number(val));
                tiles[i][j]->setStyleSheet(Theme::getTileStyle(val));
            }
        }
    }
    return boardChanged;
}

bool GameBoard::slideLeft() 
{
    bool boardChanged = false;

    for (int i = 0; i < rowCount; ++i) {
        
        QVector<int> line;
        for (int j = 0; j < colCount; ++j) {
            line.append(getTileValue(i, j));
        }

        bool rowChanged = mergeLine(line);
        if (rowChanged) {
            boardChanged = true;
            
            for (int j = 0; j < colCount; ++j) {
                int val = line[j];
                tiles[i][j]->setText(val == 0 ? "" : QString::number(val));
                tiles[i][j]->setStyleSheet(Theme::getTileStyle(val));
            }
        }
    }
    return boardChanged;
}

bool GameBoard::mergeLine(QVector<int>& line) 
{
    QVector<int> originalLine = line;
    int length = line.size();

    // Get rid of zeros
    QVector<int> compressed;
    for (int i = 0; i < length; ++i) {
        if (line[i] != 0) {
            compressed.append(line[i]);
        }
    }

    // Merge adjacent
    for (int i = 0; i < compressed.size() - 1; ++i) {
        if (compressed[i] == compressed[i+1]) {
            compressed[i] *= 2;
            compressed[i+1] = 0;

            // Track score
            int gain = compressed[i];

            currentScore += gain;

            emit scoreUpdate(currentScore);

            if (currentScore > bestScore) {
                bestScore = currentScore;
                emit bestScoreUpdate(bestScore);
            }
        }
    }

    // Get rid of zeros again
    QVector<int> finalLine;
    for (int i = 0; i < compressed.size(); ++i) {
        if (compressed[i] != 0) {
            finalLine.append(compressed[i]);
        }
    }

    // Pad zeros
    while (finalLine.size() < length) {
        finalLine.append(0);
    }

    line = finalLine;

    // If the line changed at all, return true!
    return (originalLine != line);
}

GameState GameBoard::captureCurrentState()
{
    GameState state;
    state.score = currentScore;
    state.isGameOver = isEnded;
    state.boardState.resize(rowCount);

    for (int i = 0; i < rowCount; ++i) {
        state.boardState[i].resize(rowCount);
        for (int j = 0; j < colCount; ++j) {
            state.boardState[i][j] = getTileValue(i, j);
        }
    }
    return state;
}

void GameBoard::undo()
{
    if (undoStack.isEmpty()) {
        return;
    }

    // Pop last value
    GameState previousState = undoStack.top();
    undoStack.pop();

    // Restore score
    currentScore = previousState.score;
    emit scoreUpdate(currentScore);

    // Restore flag
    isEnded = previousState.isGameOver;

    // Restore grid
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            int val = previousState.boardState[i][j];
            tiles[i][j]->setText(val == 0 ? "" : QString::number(val));
            tiles[i][j]->setStyleSheet(Theme::getTileStyle(val));
        }
    }

    // Restart timer
    if (currentMode == GameMode::Hard && !isEnded) {
        hardModeTimer->start(5000);
    }
}

bool GameBoard::checkWin()
{
    if (currentMode == GameMode::Unlimited) {
        return false;
    }

    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            if (getTileValue(i, j) == K) {
                qDebug() << "Game Won";
                return true;
            }
        }
    }
    return false;
}

bool GameBoard::checkLose()
{

    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            // Check empty spot
            if (getTileValue(i, j) == 0) return false;

            // Check right
            if (j < colCount - 1 && getTileValue(i, j) == getTileValue(i, j + 1)) return false;

            // Check down
            if (i < rowCount - 1 && getTileValue(i, j) == getTileValue(i + 1, j)) return false;
        }
    }

    qDebug() << "Game Lost";
    return true;
}

void GameBoard::triggerLoseGame(const QString &title, const QString &message)
{
    isEnded = true;
    hardModeTimer->stop();

    QMessageBox overlay(this);

    overlay.setStyleSheet(Theme::getMessageBoxStyle());

    overlay.setWindowTitle(title);
    overlay.setText(title);
    overlay.setInformativeText(message + "\n\n Restart or undo, or choose a new gamemode.");

    overlay.exec();
}

void GameBoard::updateTimerUI()
{
    if (currentMode == GameMode::Hard && hardModeTimer->isActive()) {
        int remainingMs = hardModeTimer->remainingTime();

        // Get seconds
        double secondsLeft = remainingMs / 1000.0;

        timerLabel->setText(QString::number(secondsLeft, 'f', 2));
    }
}

void GameBoard::setGameMode(GameMode mode)
{
    if (currentMode == mode) return;

    currentMode = mode;
    
    emit modeChange(mode); 

    // Handle Timers
    if (currentMode == GameMode::Hard && !isEnded) {
        timerLabel->setText("5.00");
        timerLabel->show();
        hardModeTimer->start(5000);
        uiTimer->start(30);
    } else {
        timerLabel->hide();
        hardModeTimer->stop();
        uiTimer->stop();
    }
}

void GameBoard::triggerWinGame()
{
    isEnded = true;
    hardModeTimer->stop();

    QMessageBox overlay(this);
    overlay.setWindowTitle("You Win!");
    overlay.setText("<center><b>You Win!</b></center>");
    overlay.setInformativeText("<center>You reached the target tile!<br><br>Do you want to continue playing in Unlimited Mode?</center>");
    
    overlay.setStyleSheet(Theme::getMessageBoxStyle());

    // Add custom choice buttons
    QPushButton *continueButton = overlay.addButton("Continue", QMessageBox::AcceptRole);
    QPushButton *restartButton = overlay.addButton("Restart Game", QMessageBox::RejectRole);

    overlay.exec();

    // Check which button they clicked
    if (overlay.clickedButton() == continueButton) {
        // Un-freeze the game and seamlessly switch to unlimited mode
        isEnded = false;
        setGameMode(GameMode::Unlimited);
    } else if (overlay.clickedButton() == restartButton) {
        // Start a brand new game
        startGame(currentMode); 
    }
}
