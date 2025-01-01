#include "LargeFileViewer.h"

LargeFileViewer::LargeFileViewer(QWidget *parent) : QPlainTextEdit(parent) {
    setReadOnly(true); // Set the viewer as read-only

    // Connect scrollbar events to handle user scrolling
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &LargeFileViewer::onScroll);
}

LargeFileViewer::~LargeFileViewer() {
    if (file.isOpen()) {
        file.close();
    }
}

bool LargeFileViewer::loadFile(const QString &filePath, int maxLinesToShow) {
    file.setFileName(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    // Clear the viewer and initialize variables
    clear();
    fileSize = file.size();
    currentOffset = 0;
    totalLinesRead = 0;
    this->maxLinesToShow = maxLinesToShow;

    // Load the initial chunk
    loadChunk(currentOffset, maxLinesToShow);
    return true;
}

void LargeFileViewer::onScroll() {
    int scrollValue = verticalScrollBar()->value();
    int maxScroll = verticalScrollBar()->maximum();

    // If user scrolls near the bottom, load the next chunk
    if (scrollValue > maxScroll - 10 && currentOffset < fileSize) {
        loadChunk(currentOffset);
    }
}

void LargeFileViewer::loadChunk(qint64 offset, int maxLines) {
    if (!file.isOpen() || offset >= fileSize) {
        return;
    }

    file.seek(offset);
    QTextStream in(&file);

    QString content;
    qint64 bytesRead = 0;
    int linesRead = 0;

    // Read line by line
    while (!in.atEnd() && bytesRead < chunkSize) {
        QString line = in.readLine();

        // Stop if we reach the maximum lines to show (if specified)
        if (maxLines > 0 && totalLinesRead >= maxLines) {
            break;
        }

        content += line + "\n";
        bytesRead += line.toUtf8().size();
        linesRead++;
        totalLinesRead++;

        // Append smaller chunks to prevent UI freezes
        if (linesRead % 100 == 0) {
            appendPlainText(content);
            content.clear();
        }
    }

    // Append remaining content
    if (!content.isEmpty()) {
        appendPlainText(content);
    }

    // Update the current offset
    currentOffset += bytesRead;

    // Disconnect scroll signal if max lines are reached
    if (maxLines > 0 && totalLinesRead >= maxLines) {
        disconnect(verticalScrollBar(), &QScrollBar::valueChanged, this, &LargeFileViewer::onScroll);
    }
}
