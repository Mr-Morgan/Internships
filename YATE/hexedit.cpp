#include "hexedit.h"
#include <QDebug>


HexEdit::HexEdit(Appearance *appearance, Buffer *buffer) : m_buffer(buffer), m_appearance(appearance)
{
    lineNumberAreaHex = new LineNumberAreaHex(this);
    sizeOfFragment = 200;  // количество строк загружаемого фрагмента
    firstLineNumberLoaded = 0; // номер первой строки загруженного фрагмента
    lastLineNumberLoaded = 0; // номер последней строки загруженного фрагмента
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // диапазон default скроллбара - фрагмент
    newVertScrollBar = new QScrollBar(this); // диапазон этого нового скроллбара - весь файл
    newVertScrollBar->setEnabled(false);
    connect(this, &HexEdit::blockCountChanged, this, &HexEdit::updateLineNumberAreaWidth);
    connect(this, &HexEdit::updateRequest, this, &HexEdit::updateLineNumberArea);
    connect(this, &HexEdit::cursorPositionChanged, this, &HexEdit::highlightCurrentLine);

/*    Подключение сигналов со скролбаров */
    connect(newVertScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollContent(int)));
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &HexEdit::syncScrollBars);

    setFont(QFont("Courier", 10));
    setOverwriteMode(true);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setAttribute(Qt::WA_DeleteOnClose);

    connect(m_buffer, &Buffer::openFile, this, &HexEdit::loadFile);
    connect(m_buffer, &Buffer::errorWithFile, this, &HexEdit::showErrorMsg);
    setReadOnly(true);
    if (!m_buffer->pathFile().isEmpty()) { m_buffer->setPathFile(m_buffer->pathFile()); }

}

HexEdit::~HexEdit()
{
    emit deleteBuffer(m_buffer);
}

int HexEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 20 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * 10;
    return space;
}

Buffer *HexEdit::buffer() const
{
    return m_buffer;
}

void HexEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
void HexEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberAreaHex->scroll(0, dy);
    else
        lineNumberAreaHex->update(0, rect.y(), lineNumberAreaHex->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void HexEdit::seekAndConvert(qint64 fromGlobalPos, qint64 size)
{
    QByteArray dataIn = m_buffer->getTextFragment(fromGlobalPos, size);
    QString result;
    for (int i = 0, k = 0; i < dataIn.size(); i += 16, ++k)
    {
        QString hexStr;
        QString ascStr;
        for (int j=0; j<16; j++)
        {
            if ((i + j) < dataIn.size())
            {
                hexStr.append(" ").append(dataIn.mid(i + j, 1).toHex());
                char ch = dataIn[i + j];
                if ((ch < 0x20) || (ch > 0x7e)){ ch = '.'; }
                ascStr.append(QChar(ch));
            }
        }
        result.append(QString("%1").arg(hexStr)+  "       " + QString("%1").arg(ascStr) + "\n");
    }
    result.chop(1);
    if (globalLinesCount - fromGlobalPos < size){
        qint32 offsetLastLine = (16 - m_buffer->getFileSize() % 16) * 3;
        result.insert(result.lastIndexOf("    "), QString(offsetLastLine, ' '));
    }
    replaceTextBlock(result, fromGlobalPos, size);
}

void HexEdit::replaceTextBlock(const QString &newTextBlock, qint64 loadedFromPos, qint64 size)
{
    firstLineNumberLoaded = loadedFromPos;
    lastLineNumberLoaded = loadedFromPos + size;
    setPlainText(newTextBlock);
}

void HexEdit::scrollContent(int verticalSBvalue)
{
    QString tmp;

    if((verticalSBvalue > firstLineNumberLoaded) && (verticalSBvalue <= lastLineNumberLoaded - rowsOnScreen()) )
    {
        verticalScrollBar()->setValue(verticalSBvalue-firstLineNumberLoaded);
    } else {
        seekAndConvert(verticalSBvalue, sizeOfFragment);
    }


}

void HexEdit::syncScrollBars(int verticalSBvalue)
{
    newVertScrollBar->blockSignals(true);
    newVertScrollBar->setValue(firstLineNumberLoaded+verticalSBvalue);
    QString tmp;
    newVertScrollBar->blockSignals(false);
}

void HexEdit::showErrorMsg(const QString &err)
{
    QMessageBox::warning(this, tr("MDI"), err);
}

void HexEdit::loadFile()
{
    globalLinesCount = (m_buffer->getFileSize() % 16 == 0) ? m_buffer->getFileSize() / 16 : m_buffer->getFileSize() / 16 + 1;
    qDebug() << "File read - ok";
    qDebug() << "Size of file: " << m_buffer->getFileSize() ;
    qDebug() << "Lines in file: " << globalLinesCount;
    seekAndConvert(0, sizeOfFragment);
    newVertScrollBar->setRange(0, globalLinesCount);
    newVertScrollBar->setEnabled(true);
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(m_buffer->fileName() + "[*]");
}



qint32 HexEdit::rowsOnScreen() // метод определяющий количество видимых строк на экране
{
    int result=0;
    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && block.isVisible() && top <= rect().bottom())
    {
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        result++;
    }
    return result;
}

void HexEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberAreaHex->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    newVertScrollBar->setGeometry(QRect(cr.width() - 20, cr.top(), 20, cr.height()));
}

void HexEdit::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        if (numDegrees.y() < 0) { scrollContent(newVertScrollBar->value() + 1); }
        else {
            newVertScrollBar->setValue(newVertScrollBar->value() - 1);
            scrollContent(newVertScrollBar->value());
        }
    }
    event->accept();
}

void HexEdit::keyPressEvent(QKeyEvent *event)
{
    int newPos;
    QString tmp;
    switch(event->key())
    {
    case Qt::Key_PageUp :
    case QKeySequence::MoveToPreviousPage:
        newPos=qMax(0,newVertScrollBar->value()-rowsOnScreen());
        newVertScrollBar->setValue(newPos);
        scrollContent(newPos);
        break;
    case Qt::Key_PageDown :
    case QKeySequence::MoveToNextPage:
        newPos=newVertScrollBar->value()+rowsOnScreen();
        newVertScrollBar->setValue(newPos);
        scrollContent(newPos);
        break;
    case Qt::Key_Up :
        if (newVertScrollBar->value() > 0 && verticalScrollBar()->value() == 0){
            if (newVertScrollBar->value() < sizeOfFragment) {
                verticalScrollBar()->setValue(newVertScrollBar->value());
            }
            else {
                seekAndConvert( newVertScrollBar->value() - sizeOfFragment + rowsOnScreen() - 2, sizeOfFragment);
                verticalScrollBar()->setValue(newVertScrollBar->value());
            }
        }
        QPlainTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_Down :

        if (verticalScrollBar()->value() > sizeOfFragment - rowsOnScreen() - 1){
            seekAndConvert(newVertScrollBar->value() + 1, sizeOfFragment);
        } else {
            QPlainTextEdit::keyPressEvent(event);
        }
        break;
    default:
        break;
    }
    event->accept();
}

void HexEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor;
        if(m_appearance->getStyleName() == "Light style"){
            lineColor = QColor(Qt::yellow).lighter(180);
        }else if(m_appearance->getStyleName() == "Dark style"){
            lineColor = QColor(Qt::gray).lighter(80);
        }

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


void HexEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberAreaHex);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber() + firstLineNumberLoaded;
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString("%4").toUpper().arg(QString::number(blockNumber, 16), 4, QLatin1Char('0')) + "0h:";
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberAreaHex->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
