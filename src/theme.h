#pragma once
#include <QString>

namespace Theme {

    inline QString getBoardContainerStyle() {
        return 
            "QWidget { "
            "background-color: #1a1d0d; "
            "border: 4px solid #0d0f06; "
            "border-bottom: 4px solid #2a2e16; "
            "border-radius: 0px; "
            "}";
    }

    inline QString getTileStyle(int value) {
        if (value == 0) {
            return 
                "QLabel { "
                "background-color: rgba(0, 0, 0, 0.4); "
                "border: 2px solid rgba(0, 0, 0, 0.7); "
                "border-bottom: 2px solid rgba(255, 255, 255, 0.05); "
                "border-radius: 0px; "
                "}";
        }

        QString bgColor;
        QString textColor = "#342E24"; 
        int fontSize = 32;

        switch (value) {
            // Earth and Leather tones
            case 2:    bgColor = "#E3E0C6"; break; // Bone White
            case 4:    bgColor = "#90816C"; break; // Sand/Taupe
            case 8:    bgColor = "#5D4532"; textColor = "#E3E0C6"; break; // Leather Brown

            // Armor Green progression
            case 16:   bgColor = "#AFB369"; break; // Light Armor
            case 32:   bgColor = "#646637"; textColor = "#E3E0C6"; break; // Dark Armor
            
            // --- NEW DISTINCT HIGH-TIER PALETTE ---
            
            // Dark Rust (From Image)
            case 64:   bgColor = "#692f17"; textColor = "#E3E0C6"; break; 

            // UAC Green (From Image)
            case 128:  bgColor = "#95a94e"; textColor = "#342E24"; fontSize = 28; break; 

            // Heavy Bronze (From Image)
            case 256:  bgColor = "#8b4c0d"; textColor = "#E3E0C6"; fontSize = 28; break; 

            // Hellfire Orange (From Image)
            case 512:  bgColor = "#cb5e29"; textColor = "#E3E0C6"; fontSize = 28; break; 

            // Sentinel Gold (Fitting tone)
            case 1024: bgColor = "#FFAB33"; textColor = "#342E24"; fontSize = 24; break; 

            // Winning tile - The Ultimate Blood Red
            case 2048: 
                bgColor = "#CD211A"; 
                textColor = "#E3E0C6"; 
                fontSize = 26; 
                break;

            // Unlimited mode (Rip and Tear)
            default:   
                bgColor = "#E3E0C6"; 
                textColor = "#CD211A"; 
                fontSize = 24; 
                break;
        }

        return QString(
            "QLabel {"
            "background-color: %1;"
            "color: %2;"
            "font-family: 'Impact', 'Arial Black', sans-serif;" 
            "font-size: %3px;"
            "border: 2px solid rgba(0, 0, 0, 0.5);"
            "border-top: 3px solid rgba(255, 255, 255, 0.4);"
            "border-radius: 0px;"
            "}"
        ).arg(bgColor, textColor, QString::number(fontSize));
    }

    inline QString getTimerStyle() {
        return 
            "QLabel { "
            "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #CD211A, stop:0.5 #901712, stop:1 #CD211A); "
            "color: #FFFFFF; "
            "font-family: 'Impact', 'Arial Black', sans-serif; "
            "font-size: 24px; "
            "font-weight: bold; "
            "border: 2px solid #70100D; "
            "border-top: 3px solid #FF4A43; "
            "border-radius: 0px; "
            "padding: 5px; "
            "}";
    }

    inline QString buttonDoomButtonStyle() {
        return 
            "QPushButton { "
            "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #252815, stop:0.5 #1a1d0d, stop:1 #252815); " 
            "border: 2px solid #8E9C3C; "       
            "border-top: 3px solid #A8B84B; "   
            "border-radius: 0px; "              
            "color: #FFFFFF; "                  
            "font-family: 'Impact', 'Arial Black', sans-serif; " 
            "font-size: 16px; "
            "font-weight: bold; "
            "padding: 8px 20px; "
            "}"
            "QPushButton:hover { "
            "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3A4020, stop:0.5 #2A3015, stop:1 #3A4020); "
            "border: 2px solid #C0D05A; " 
            "}"
            "QPushButton:pressed { "
            "background-color: #8E9C3C; " 
            "color: #000000; "
            "}";
    }

    inline QString labelDoomButtonStyle() {
        return 
            "QLabel { "
            "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #252815, stop:0.5 #1a1d0d, stop:1 #252815); " 
            "border: 2px solid #8E9C3C; "       
            "border-top: 3px solid #A8B84B; "   
            "border-radius: 0px; "              
            "color: #FFFFFF; "                  
            "font-family: 'Impact', 'Arial Black', sans-serif; " 
            "font-size: 16px; "
            "font-weight: bold; "
            "padding: 6px 15px; "
            "}"
            "QPushButton:hover { "
            "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3A4020, stop:0.5 #2A3015, stop:1 #3A4020); "
            "border: 2px solid #C0D05A; " 
            "}"
            "QPushButton:pressed { "
            "background-color: #8E9C3C; " 
            "color: #000000; "
            "}";
    }

    inline QString getModeButtonStyle(bool isSelected) {
        if (isSelected) {
            return 
                "QPushButton { "
                "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E88A1A, stop:0.5 #FFAB33, stop:1 #E88A1A); "
                "border: 2px solid #8C4E05; "
                "border-top: 3px solid #FFF0A8; " // Bright glowing top lip
                "border-radius: 0px; "
                "color: #FFFFFF; "
                "font-family: 'Impact', 'Arial Black', sans-serif; "
                "font-size: 16px; "
                "font-weight: bold; "
                "padding: 8px 20px; "
                "}";
        } else {
            // The dimmed, unselected version
            return 
                "QPushButton { "
                "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5A3305, stop:0.5 #7A480D, stop:1 #5A3305); "
                "border: 2px solid #2D1902; "
                "border-top: 3px solid #9A6018; " // Dimmer top lip
                "border-radius: 0px; "
                "color: #CFCFCF; " // Slightly dimmed text
                "font-family: 'Impact', 'Arial Black', sans-serif; "
                "font-size: 16px; "
                "font-weight: bold; "
                "padding: 8px 20px; "
                "}"
                
                // Hover effect so the user knows they can click it
                "QPushButton:hover { "
                "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7A480D, stop:0.5 #9A5A12, stop:1 #7A480D); "
                "border-top: 3px solid #D89838; "
                "color: #FFFFFF; "
                "}";
        }
    }

    inline QString getMessageBoxStyle() {
        return 
            "QMessageBox { "
            "background-color: #1a1d0d; "
            "} "
            "QMessageBox QLabel { "
            "color: #E3E0C6; "
            "font-family: 'Impact', 'Arial Black', sans-serif; "
            "font-size: 16px; "
            "} "
            "QMessageBox QPushButton { "
            "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #252815, stop:0.5 #1a1d0d, stop:1 #252815); " 
            "border: 2px solid #8E9C3C; "       
            "border-top: 3px solid #A8B84B; "   
            "border-radius: 0px; "              
            "color: #FFFFFF; "                  
            "font-family: 'Impact', 'Arial Black', sans-serif; " 
            "font-size: 14px; "
            "padding: 6px 15px; "
            "min-width: 80px; "
            "} "
            "QMessageBox QPushButton:hover { "
            "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3A4020, stop:0.5 #2A3015, stop:1 #3A4020); "
            "border: 2px solid #C0D05A; " 
            "} "
            "QMessageBox QPushButton:pressed { "
            "background-color: #8E9C3C; " 
            "color: #000000; "
            "}";
    }
}