#ifndef Result_h
#define Result_h

#include <algorithm>
#include <QList>
#include <QString>

namespace Pecera
{

class Extent
{
public:
    Extent(int start, int length)
        : m_start(start)
        , m_end(m_start + length)
    {
    }

    bool operator==(const Extent& other)
    {
        return start() == other.start()
             && end() == other.end();
    }

    int start() const { return m_start; }
    int end() const { return m_end; }
    int length() const { return end() - start(); }
    void setStart(int start) { m_start = start; }
    void setEnd(int end) { m_end = end; }
    void setLength(int length) { m_end = m_start + length; }

    bool mergeIfIntersects(const Extent& extent)
    {
        if ((start() <= extent.start() && end() > extent.start())
            || (end() >= extent.end() && start() < extent.end())) {
            setStart(std::min(start(), extent.start()));
            setEnd(std::max(end(), extent.end()));
            return true;
        } else {
            return false;
        }
    }

private:
    int m_start;
    int m_end;
};

class Result
{
public:
    Result() { }
    virtual void executeAction() = 0;

    void addExtent(Extent& extent)
    {
        int i = 0;
        while (i < m_extents.size() && m_extents[i].start() < extent.start())
            i++;

        // Merge this extent and the next, if possible.
        if (i < m_extents.size()) {
            const Extent& nextExtent = m_extents[i];
            if (extent.mergeIfIntersects(nextExtent))
                m_extents.removeAt(i);
        }

        // Merge the extent with the previous, if possible.
        if (i > 0 && extent.mergeIfIntersects(m_extents[i-1])) {
            m_extents.removeAt(i-1);
            i--;
        }

        m_extents.insert(i, extent);
    }

    QList<Extent>& extents() { return m_extents; }
    const QString& text() { return m_text; }
    const QString& url() { return m_url; }
    void setText(const QString& text) { m_text = text; }

protected:
    QString m_text;
    QString m_url;
    QList<Extent> m_extents;
};

}

#endif
