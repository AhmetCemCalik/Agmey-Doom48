#include "mainwindow.h"
#include "gameboard.h"
#include "theme.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Doom48");
    resize(540, 810);

    // 1. Set up the single Central Widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setStyleSheet("QMainWindow { background-color: #1F1B15; }");

    // Main Vertical Layout for the entire window
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(15);

    // --- 2. HEADER AREA (Title & Scores) ---
    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    // Awesome 2048 title label styling 
    QLabel *titleLabel = new QLabel("2048", this);
    // 1. Set the font and the off-white text color
    titleLabel->setStyleSheet("font-size: 64px; font-weight: bold; color: #E3E0C6;");

    // 2. Create a sharp red shadow to mimic a 3D text stroke
    QGraphicsDropShadowEffect *redOutline = new QGraphicsDropShadowEffect(this);
    
    // A blur radius of 0 ensures the edge is sharp and pixelated, not fuzzy
    redOutline->setBlurRadius(0); 
    redOutline->setColor(QColor("#CD211A"));
    
    // Offset the shadow slightly to make it pop like a border
    redOutline->setOffset(2, 2); 

    // 3. Apply the effect to the label
    titleLabel->setGraphicsEffect(redOutline);
    
    // Placeholder score labels (you can style these inside dark boxes later)
    QLabel *scoreLabel = new QLabel("SCORE\n0", this);
    QLabel *bestLabel  = new QLabel("BEST\n0", this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet(Theme::labelDoomButtonStyle());
    bestLabel->setAlignment(Qt::AlignCenter);
    bestLabel->setStyleSheet(Theme::labelDoomButtonStyle());
    
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch(); // This pushes the title left and scores right
    headerLayout->addWidget(scoreLabel);
    headerLayout->addWidget(bestLabel);

    // --- 3. CONTROLS AREA (Modes & Actions) ---
    QHBoxLayout *modeLayout = new QHBoxLayout();
    auto *normalModeButton    = new QPushButton("Normal", this);
    auto *unlimitedModeButton = new QPushButton("Unlimited", this);
    auto *hardModeButton      = new QPushButton("Hard", this);
    
    modeLayout->addWidget(normalModeButton);
    modeLayout->addWidget(unlimitedModeButton);
    modeLayout->addWidget(hardModeButton);
    modeLayout->addStretch(); // Aligns buttons to the left

    QHBoxLayout *actionLayout = new QHBoxLayout();
    auto *undoButton    = new QPushButton("Undo (U)", this);
    auto *restartButton = new QPushButton("Restart (R)", this);

    undoButton->setShortcut(QKeySequence(Qt::Key_U));
    restartButton->setShortcut(QKeySequence(Qt::Key_R));
    
    actionLayout->addWidget(undoButton);
    actionLayout->addWidget(restartButton);
    actionLayout->addStretch();

    
    auto updateButtonColors = [=](GameMode activeMode) {
        normalModeButton->setStyleSheet(Theme::getModeButtonStyle(activeMode == GameMode::Normal));
        unlimitedModeButton->setStyleSheet(Theme::getModeButtonStyle(activeMode == GameMode::Unlimited));
        hardModeButton->setStyleSheet(Theme::getModeButtonStyle(activeMode == GameMode::Hard));
    };

    undoButton->setStyleSheet(Theme::buttonDoomButtonStyle());
    restartButton->setStyleSheet(Theme::buttonDoomButtonStyle());


    // --- 4. GAME BOARD AREA ---
    gameBoard = new GameBoard(this);

    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(gameBoard);

    // --- CONNECTIONS ---
    connect(normalModeButton, &QPushButton::clicked, this, [this]() {
        gameBoard->setGameMode(GameMode::Normal);
        gameBoard->setFocus();
    });
    connect(unlimitedModeButton, &QPushButton::clicked, this, [this]() {
        gameBoard->setGameMode(GameMode::Unlimited);
        gameBoard->setFocus();
    });
    connect(hardModeButton, &QPushButton::clicked, this, [this]() {
        gameBoard->setGameMode(GameMode::Hard);
        gameBoard->setFocus();
    });
    connect(gameBoard, &GameBoard::modeChange, this, updateButtonColors);

    connect(restartButton, &QPushButton::clicked, this, [this]() {
        gameBoard->startGame(gameBoard->getMode());
        gameBoard->setFocus();
    });
    connect(undoButton, &QPushButton::clicked, this, [this]() {
        gameBoard->undo();
        gameBoard->setFocus();
    });
    // SCOREBOARD
    connect(gameBoard, &GameBoard::scoreUpdate, this, [scoreLabel](int newScore) {
        scoreLabel->setText(QString("SCORE\n%1").arg(newScore));
    });
    connect(gameBoard, &GameBoard::bestScoreUpdate, this, [bestLabel](int bestScore) {
        bestLabel->setText(QString("BEST\n%1").arg(bestScore));
    });

    // --- INITIALIZATION ---
    // Because the board is immediately visible, we must draw it on startup
    gameBoard->startGame(GameMode::Normal);
    updateButtonColors(GameMode::Normal);
}