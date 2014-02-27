/***************************************************************************
 *   Copyright (C) 2014 by Frank Reininghaus <frank78ac@googlemail.com>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#include <kio/udsentry.h>

#include <QTest>

/**
 * This benchmarks tests four typical uses of UDSEntry:
 *
 * (a)  Store data in UDSEntries using
 *      UDSEntry::insert(uint, const QString&) and
 *      UDSEntry::insert(uint, long long),
 *      and append the entries to a UDSEntryList.
 *
 * (b)  Read data from UDSEntries in a UDSEntryList using
 *      UDSEntry::stringValue(uint) and UDSEntry::numberValue(uint).
 *
 * (c)  Save a UDSEntryList in a QDataStream.
 *
 * (d)  Load a UDSEntryList from a QDataStream.
 *
 * This is done for two different data sets:
 *
 * 1.   UDSEntries containing the entries which are provided by kio_file.
 *
 * 2.   UDSEntries with a larger number of "fields".
 */

// The following constants control the number of UDSEntries that are considered
// in each test, and the number of extra "fields" that are used for large UDSEntries.
const int numberOfSmallUDSEntries = 100 * 1000;
const int numberOfLargeUDSEntries = 5 * 1000;
const int extraFieldsForLargeUDSEntries = 40;


class UDSEntryBenchmark : public QObject
{
    Q_OBJECT

public:
    UDSEntryBenchmark();

private Q_SLOTS:
    void createSmallEntries();
    void createLargeEntries();
    void readFieldsFromSmallEntries();
    void readFieldsFromLargeEntries();
    void saveSmallEntries();
    void saveLargeEntries();
    void loadSmallEntries();
    void loadLargeEntries();

private:
    KIO::UDSEntryList m_smallEntries;
    KIO::UDSEntryList m_largeEntries;
    QByteArray m_savedSmallEntries;
    QByteArray m_savedLargeEntries;

    QVector<uint> m_fieldsForLargeEntries;
};

UDSEntryBenchmark::UDSEntryBenchmark()
{
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_SIZE);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_SIZE_LARGE);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_USER);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_ICON_NAME);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_GROUP);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_NAME);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_LOCAL_PATH);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_HIDDEN);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_ACCESS);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_MODIFICATION_TIME);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_ACCESS_TIME);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_CREATION_TIME);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_FILE_TYPE);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_LINK_DEST);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_URL);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_MIME_TYPE);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_GUESSED_MIME_TYPE);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_XML_PROPERTIES);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_EXTENDED_ACL);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_ACL_STRING);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_DEFAULT_ACL_STRING);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_DISPLAY_NAME);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_TARGET_URL);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_DISPLAY_TYPE);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_ICON_OVERLAY_NAMES);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_COMMENT);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_DEVICE_ID);
    m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_INODE);

    for (int i = 0; i < extraFieldsForLargeUDSEntries; ++i) {
        m_fieldsForLargeEntries.append(KIO::UDSEntry::UDS_EXTRA + i);
    }
}

void UDSEntryBenchmark::createSmallEntries()
{
    m_smallEntries.clear();
    m_smallEntries.reserve(numberOfSmallUDSEntries);

    QBENCHMARK_ONCE {
        for (int i = 0; i < numberOfSmallUDSEntries; ++i) {
            KIO::UDSEntry entry;
            entry.insert(KIO::UDSEntry::UDS_NAME, QString::number(i));
            entry.insert(KIO::UDSEntry::UDS_FILE_TYPE, i);
            entry.insert(KIO::UDSEntry::UDS_ACCESS, i);
            entry.insert(KIO::UDSEntry::UDS_SIZE, i);
            entry.insert(KIO::UDSEntry::UDS_MODIFICATION_TIME, i);
            entry.insert(KIO::UDSEntry::UDS_USER, QLatin1String("user"));
            entry.insert(KIO::UDSEntry::UDS_GROUP, QLatin1String("group"));
            entry.insert(KIO::UDSEntry::UDS_ACCESS_TIME, i);
            m_smallEntries.append(entry);
        }
    }

    Q_ASSERT(m_smallEntries.count() == numberOfSmallUDSEntries);
}

void UDSEntryBenchmark::createLargeEntries()
{
    m_largeEntries.clear();
    m_largeEntries.reserve(numberOfLargeUDSEntries);

    QBENCHMARK_ONCE {
        for (int i = 0; i < numberOfLargeUDSEntries; ++i) {
            KIO::UDSEntry entry;
            foreach (uint field, m_fieldsForLargeEntries) {
                if (field & KIO::UDSEntry::UDS_STRING) {
                    entry.insert(field, QString::number(i));
                } else {
                    entry.insert(field, i);
                }
            }
            m_largeEntries.append(entry);
        }
    }

    Q_ASSERT(m_largeEntries.count() == numberOfLargeUDSEntries);
}

void UDSEntryBenchmark::readFieldsFromSmallEntries()
{
    // Create the entries if they do not exist yet.
    if (m_smallEntries.isEmpty()) {
        createSmallEntries();
    }

    QBENCHMARK_ONCE {
        long long i = 0;

        foreach (const KIO::UDSEntry& entry, m_smallEntries) {
            QCOMPARE(entry.count(), 8);

            QCOMPARE(QString::number(i), entry.stringValue(KIO::UDSEntry::UDS_NAME));
            QCOMPARE(i, entry.numberValue(KIO::UDSEntry::UDS_FILE_TYPE));
            QCOMPARE(i, entry.numberValue(KIO::UDSEntry::UDS_ACCESS));
            QCOMPARE(i, entry.numberValue(KIO::UDSEntry::UDS_SIZE));
            QCOMPARE(i, entry.numberValue(KIO::UDSEntry::UDS_MODIFICATION_TIME));
            QCOMPARE(QLatin1String("user"), entry.stringValue(KIO::UDSEntry::UDS_USER));
            QCOMPARE(QLatin1String("group"), entry.stringValue(KIO::UDSEntry::UDS_GROUP));
            QCOMPARE(i, entry.numberValue(KIO::UDSEntry::UDS_ACCESS_TIME));
            ++i;
        }

        QCOMPARE(i, numberOfSmallUDSEntries);
    }
}

void UDSEntryBenchmark::readFieldsFromLargeEntries()
{
    // Create the entries if they do not exist yet.
    if (m_largeEntries.isEmpty()) {
        createLargeEntries();
    }

    QBENCHMARK_ONCE {
        long long i = 0;

        foreach (const KIO::UDSEntry& entry, m_largeEntries) {
            QCOMPARE(entry.count(), m_fieldsForLargeEntries.count());

            foreach (uint field, m_fieldsForLargeEntries) {
                if (field & KIO::UDSEntry::UDS_STRING) {
                    QCOMPARE(entry.stringValue(field), QString::number(i));;
                } else {
                    QCOMPARE(entry.numberValue(field), i);
                }
            }

            ++i;
        }

        QCOMPARE(i, numberOfLargeUDSEntries);
    }
}

void UDSEntryBenchmark::saveSmallEntries()
{
    // Create the entries if they do not exist yet.
    if (m_smallEntries.isEmpty()) {
        createSmallEntries();
    }

    m_savedSmallEntries.clear();

    QBENCHMARK_ONCE {
        QDataStream stream(&m_savedSmallEntries, QIODevice::WriteOnly);
        stream << m_smallEntries;
    }
}

void UDSEntryBenchmark::saveLargeEntries()
{
    // Create the entries if they do not exist yet.
    if (m_smallEntries.isEmpty()) {
        createLargeEntries();
    }

    m_savedLargeEntries.clear();

    QBENCHMARK_ONCE {
        QDataStream stream(&m_savedLargeEntries, QIODevice::WriteOnly);
        stream << m_largeEntries;
    }
}

bool operator==(const KIO::UDSEntry &a, const KIO::UDSEntry &b)
{
    if (a.count() != b.count()) {
        return false;
    }

    const QList<uint> fields = a.listFields();
    foreach (uint field, fields) {
        if (!b.contains(field)) {
            return false;
        }

        if (field & KIO::UDSEntry::UDS_STRING) {
            if (a.stringValue(field) != b.stringValue(field)) {
                return false;
            }
        } else {
            if (a.numberValue(field) != b.numberValue(field)) {
                return false;
            }
        }
    }

    return true;
}

void UDSEntryBenchmark::loadSmallEntries()
{
    // Save the entries if that has not been done yet.
    if (m_savedSmallEntries.isEmpty()) {
        saveSmallEntries();
    }

    QDataStream stream(m_savedSmallEntries);
    KIO::UDSEntryList entries;

    QBENCHMARK_ONCE {
        stream >> entries;
    }

    QCOMPARE(entries, m_smallEntries);
}

void UDSEntryBenchmark::loadLargeEntries()
{
    // Save the entries if that has not been done yet.
    if (m_savedLargeEntries.isEmpty()) {
        saveLargeEntries();
    }

    QDataStream stream(m_savedLargeEntries);
    KIO::UDSEntryList entries;

    QBENCHMARK_ONCE {
        stream >> entries;
    }

    QCOMPARE(entries, m_largeEntries);
}


QTEST_MAIN(UDSEntryBenchmark)

#include "udsentrybenchmark.moc"
