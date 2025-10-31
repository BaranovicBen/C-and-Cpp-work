#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <compare>
#include <functional>
#include <optional>

struct CTimeStamp {
    int year;
    int month;
    int day;
    int hour;
    int min;
    double sec;

    CTimeStamp() {
        year = 2025;
        month = 1;
        day = 1;
        hour = 0;
        min = 0;
        sec = 0.0;
    }

    CTimeStamp(int ye,int mo,int da, int ho,int mi, double se){
        year = ye;
        month = mo;
        day = da;
        hour = ho;
        min = mi;
        sec = se;
    }

    int compare(const CTimeStamp& other) const {
        if (year != other.year) return year - other.year;
        if (month != other.month) return month - other.month;
        if (day != other.day) return day - other.day;
        if (hour != other.hour) return hour - other.hour;
        if (min != other.min) return min - other.min;
        if (sec != other.sec) return (sec > other.sec) - (sec < other.sec);
        return 0;
    }

    bool operator<(const CTimeStamp& other) const {
        return compare(other) < 0;
    }

    bool operator==(const CTimeStamp& other) const {
        return compare(other) == 0;
    }
};

std::ostream& operator<<(std::ostream& os, const CTimeStamp& ts) {
    os << std::setfill('0')
       << std::setw(4) << ts.year << '-'
       << std::setw(2) << ts.month << '-'
       << std::setw(2) << ts.day << ' '
       << std::setw(2) << ts.hour << ':'
       << std::setw(2) << ts.min << ':'
       << std::fixed << std::setprecision(3) << std::setw(6) << ts.sec;
    return os;
};

struct CMail{
    CTimeStamp mailTimeStamp;
    std::string mailFrom;
    std::string mailRecipient;
    std::optional<std::string> mailSbjct;

    CMail(const CTimeStamp& timeStamp, const std::string& from, const std::string& to, std::optional<std::string> subject = std::nullopt)
    : mailTimeStamp(timeStamp), mailFrom(from), mailRecipient(to), mailSbjct(subject) {}

    int compareByTime(const CTimeStamp& x) const {
        return mailTimeStamp.compare(x);
    }

    int compareByTime(const CMail& x) const {
        return mailTimeStamp.compare(x.mailTimeStamp);
    }

    const std::string& from() const { return mailFrom; }
    const std::string& to() const { return mailRecipient; }
    const std::optional<std::string>& subject() const { return mailSbjct; }
    const CTimeStamp& timeStamp() const { return mailTimeStamp; }
};

std::ostream& operator<<(std::ostream& os, const CMail& x) {
    os << x.timeStamp() << " " << x.from() << " -> " << x.to();
    if (x.subject().has_value())
        os << ", subject: " << x.subject().value();
    return os;
}
namespace MysteriousNamespace {
#endif /* __PROGTEST__ */


struct CMailLog {
    int parseLog(std::istream& in) {
        std::string line;
        int deliveredCount = 0;
    
        std::unordered_map<std::string, std::string> fromMap;
        std::unordered_map<std::string, std::optional<std::string>> subjectMap;
    
        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string monthStr, mailID, relay, message;
            int day, year, hour, minute;
            double second;
    
            if (!(iss >> monthStr >> day >> year >> hour)) continue;
            if (iss.get() != ':') continue;
            if (!(iss >> minute)) continue;
            if (iss.get() != ':') continue;
            if (!(iss >> second)) continue;
    
            if (!(iss >> relay >> mailID)) continue;
            if (mailID.back() != ':') continue;
            mailID.pop_back();
    
            std::getline(iss, message);
            CTimeStamp ts(year, convertMonth(monthStr), day, hour, minute, second);
    
            if (message.starts_with(" from=")) {
                fromMap[mailID] = message.substr(6); // skip " from="
            } else if (message.starts_with(" subject=")) {
                subjectMap[mailID] = message.substr(9); // skip " subject="
            } else if (message.starts_with(" to=")) {
                std::string recipient = message.substr(4); // skip " to="
                auto fromIt = fromMap.find(mailID);
                if (fromIt != fromMap.end()) {
                    auto subjIt = subjectMap.find(mailID);
                    CMail newMail(ts, fromIt->second, recipient,
                                  subjIt != subjectMap.end() ? subjIt->second : std::nullopt);
                    auto pos = std::upper_bound(
                        allMailsSorted.begin(), allMailsSorted.end(), newMail,
                        [](const CMail& a, const CMail& b) {
                            return a.compareByTime(b) < 0;
                        });
                    allMailsSorted.insert(pos, newMail);
                    deliveredCount++;
                }
            }
        }
    
        return deliveredCount;
    }
    
        std::list<CMail> listMail(const CTimeStamp& from, const CTimeStamp& to) const {
            std::list<CMail> result;
            for (const auto& mail : allMailsSorted) {
                if (mail.compareByTime(from) >= 0 && mail.compareByTime(to) <= 0) {
                    result.push_back(mail);
                }
            }
            return result;
        }
    
        std::set<std::string> activeUsers(const CTimeStamp& from, const CTimeStamp& to) const {
            std::set<std::string> result;
            for (const auto& mail : allMailsSorted) {
                if (mail.compareByTime(from) >= 0 && mail.compareByTime(to) <= 0) {
                    result.insert(mail.from());
                    result.insert(mail.to());
                }
            }
            return result;
        }
    
        std::vector<CMail> allMailsSorted;
    
        std::string trim(const std::string& str) {
            size_t start = str.find_first_not_of(" \t");
            size_t end = str.find_last_not_of(" \t");
            if (start == std::string::npos || end == std::string::npos)
                return "";
            return str.substr(start, end - start + 1);
        }
    
        int convertMonth(const std::string& month) {
            static const std::map<std::string, int> monthMap = {
                {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5},
                {"Jun", 6}, {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10},
                {"Nov", 11}, {"Dec", 12}
            };
            auto it = monthMap.find(month);
            return it != monthMap.end() ? it->second : 0;
        }
    };    
#ifndef __PROGTEST__
}
std::string             printMail       ( const std::list<CMail> & all )
{
  std::ostringstream oss;
  for ( const auto & mail : all )
    oss << mail << "\n";
  return oss . str ();
}
int                     main ()
{
  MysteriousNamespace::CMailLog m;
  std::list<CMail> mailList;
  std::set<std::string> users;
  std::istringstream iss;

  iss . clear ();
  iss . str (
    "Mar 29 2025 12:35:32.233 relay.fit.cvut.cz ADFger72343D: from=user1@fit.cvut.cz\n"
    "Mar 29 2025 12:37:16.234 relay.fit.cvut.cz JlMSRW4232Df: from=person3@fit.cvut.cz\n"
    "Mar 29 2025 12:55:13.023 relay.fit.cvut.cz JlMSRW4232Df: subject=New progtest homework!\n"
    "Mar 29 2025 13:38:45.043 relay.fit.cvut.cz Kbced342sdgA: from=office13@fit.cvut.cz\n"
    "Mar 29 2025 13:36:13.023 relay.fit.cvut.cz JlMSRW4232Df: to=user76@fit.cvut.cz\n"
    "Mar 29 2025 13:55:31.456 relay.fit.cvut.cz KhdfEjkl247D: from=PR-department@fit.cvut.cz\n"
    "Mar 29 2025 14:18:12.654 relay.fit.cvut.cz Kbced342sdgA: to=boss13@fit.cvut.cz\n"
    "Mar 29 2025 14:48:32.563 relay.fit.cvut.cz KhdfEjkl247D: subject=Business partner\n"
    "Mar 29 2025 14:58:32.000 relay.fit.cvut.cz KhdfEjkl247D: to=HR-department@fit.cvut.cz\n"
    "Mar 29 2025 14:25:23.233 relay.fit.cvut.cz ADFger72343D: mail undeliverable\n"
    "Mar 29 2025 15:02:34.231 relay.fit.cvut.cz KhdfEjkl247D: to=CIO@fit.cvut.cz\n"
    "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=CEO@fit.cvut.cz\n"
    "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=dean@fit.cvut.cz\n"
    "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=vice-dean@fit.cvut.cz\n"
    "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=archive@fit.cvut.cz\n" );
  assert ( m . parseLog ( iss ) == 8 );
  mailList = m . listMail ( CTimeStamp ( 2025, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2025, 3, 29, 23, 59, 59 ) );
  assert ( printMail ( mailList ) == R"###(2025-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!
2025-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz
2025-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> CEO@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> dean@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> vice-dean@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> archive@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.231 PR-department@fit.cvut.cz -> CIO@fit.cvut.cz, subject: Business partner
)###" );
  mailList = m . listMail ( CTimeStamp ( 2025, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2025, 3, 29, 14, 58, 32 ) );
  assert ( printMail ( mailList ) == R"###(2025-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!
2025-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz
2025-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner
)###" );
  mailList = m . listMail ( CTimeStamp ( 2025, 3, 30, 0, 0, 0 ),
                            CTimeStamp ( 2025, 3, 30, 23, 59, 59 ) );
  assert ( printMail ( mailList ) == "" );
  users = m . activeUsers ( CTimeStamp ( 2025, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2025, 3, 29, 23, 59, 59 ) );
  assert ( users == std::set<std::string>( { "CEO@fit.cvut.cz", "CIO@fit.cvut.cz", "HR-department@fit.cvut.cz", "PR-department@fit.cvut.cz", "archive@fit.cvut.cz", "boss13@fit.cvut.cz", "dean@fit.cvut.cz", "office13@fit.cvut.cz", "person3@fit.cvut.cz", "user76@fit.cvut.cz", "vice-dean@fit.cvut.cz" } ) );
  users = m . activeUsers ( CTimeStamp ( 2025, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2025, 3, 29, 13, 59, 59 ) );
  assert ( users == std::set<std::string>( { "person3@fit.cvut.cz", "user76@fit.cvut.cz" } ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */