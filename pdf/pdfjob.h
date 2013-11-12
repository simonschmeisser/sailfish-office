/*
 *
 */

#ifndef PDFJOB_H
#define PDFJOB_H

#include <QString>
#include <QImage>
#include <QObject>

namespace Poppler
{
    class Document;
}

class PDFJob : public QObject
{
    Q_OBJECT
public:
    enum JobType {
        LoadDocumentJob,
        RenderPageJob,
        PageSizesJob,
    };

    PDFJob( JobType type ) : m_document{ nullptr }, m_type{ type } { }
    virtual ~PDFJob() { }

    virtual void run() = 0;

    JobType type() const { return m_type; }

protected:
    friend class PDFRenderThread;
    Poppler::Document* m_document;

private:
    JobType m_type;
};

class LoadDocumentJob : public PDFJob
{
    Q_OBJECT
public:
    LoadDocumentJob( const QString& source );

    virtual void run();

private:
    QString m_source;
};

class RenderPageJob : public PDFJob
{
    Q_OBJECT
public:
    RenderPageJob(int index, uint width);

    virtual void run();

    int m_index;
    QImage m_page;

private:
    uint m_width;
};

class PageSizesJob : public PDFJob
{
    Q_OBJECT
public:
    PageSizesJob() : PDFJob{ PDFJob::PageSizesJob } { }

    virtual void run();

    QList< QSizeF > m_pageSizes;
};

#endif // PDFJOB_H
