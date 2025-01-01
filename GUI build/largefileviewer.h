#ifndef LARGEFILEVIEWER_H
#define LARGEFILEVIEWER_H

#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>

class LargeFileViewer : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit LargeFileViewer(QWidget *parent = nullptr);
    ~LargeFileViewer();

    // Load a file into the viewer; maxLinesToShow limits the number of lines to display
    bool loadFile(const QString &filePath, int maxLinesToShow = 0);

private slots:
    // Handles user scrolling to load additional file content
    void onScroll();

private:
    QFile file;               // File object to read the content
    qint64 fileSize;          // Total size of the file
    qint64 currentOffset;     // Current offset in the file
    int totalLinesRead;       // Total number of lines read so far
    int maxLinesToShow;       // Maximum lines to display initially (0 for no limit)
    static const qint64 chunkSize = 8192; // Size of each file chunk to load (in bytes)

    // Load a chunk of the file from a specific offset
    void loadChunk(qint64 offset, int maxLines = 0);
};

#endif // LARGEFILEVIEWER_H
