#include "RecurrenceRuleDaily.h"
#include "JSONPersistence.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include "TimeUtils.h"

// Função utilitária para escapar caracteres especiais em JSON
static std::string escapeJson(const std::string& s) {
    std::ostringstream o;
    for (auto c : s) {
        switch (c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    o << "\\u"
                      << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    o << c;
                }
        }
    }
    return o.str();
}
void JSONPersistence::save(const std::vector<std::shared_ptr<Event>>& events, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("Nao foi possivel abrir arquivo para escrita: " + filename);

    ofs << "[\n";
    for (size_t i = 0; i < events.size(); ++i) {
        auto &e = events[i];
        ofs << "  {\n";
        ofs << "    \"title\": " << "\"" << escapeJson(e->getTitle()) << "\",\n";
        ofs << "    \"description\": " << "\"" << escapeJson(e->getDescription()) << "\",\n";
        ofs << "    \"start\": " << "\"" << TimeUtils::toString(e->getStartTime()) << "\",\n";
        ofs << "    \"end\": " << "\"" << TimeUtils::toString(e->getEndTime()) << "\",\n";
        ofs << "    \"tags\": [";
        const auto &tags = e->getTags();
        for (size_t ti = 0; ti < tags.size(); ++ti) {
            ofs << "\"" << escapeJson(tags[ti]) << "\"";
            if (ti + 1 < tags.size()) ofs << ", ";
        }
        ofs << "]";
        if (e->getRecurrence()) {
            ofs << ",\n    \"recurrence\": \"" << escapeJson(e->getRecurrence()->getDescription()) << "\"\n";
        } else {
            ofs << "\n";
        }
        ofs << "  }";
        if (i + 1 < events.size()) ofs << ",";
        ofs << "\n";
    }
    ofs << "]\n";
    ofs.close();
}

// very small helper to un-escape / escape (not full implementation)
static std::string unquote(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') return s.substr(1, s.size()-2);
    return s;
}

// For simplicity, this parser expects the exact structure produced by save()
std::vector<std::shared_ptr<Event>> JSONPersistence::load(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("Nao foi possivel abrir arquivo para leitura: " + filename);

    std::vector<std::shared_ptr<Event>> events;
    std::string line;
    std::string buf;
    while (std::getline(ifs, line)) {
        buf += line + "\n";
    }
    ifs.close();

    // Primitive parsing: split objects by "  {" and "  }"
    size_t pos = 0;
    while (true) {
        size_t objStart = buf.find("{", pos);
        if (objStart == std::string::npos) break;
        size_t objEnd = buf.find("}", objStart);
        if (objEnd == std::string::npos) break;
        std::string obj = buf.substr(objStart, objEnd - objStart + 1);
        // extract fields
        auto getField = [&](const std::string& key)->std::string{
            size_t k = obj.find("\"" + key + "\"");
            if (k == std::string::npos) return "";
            size_t colon = obj.find(":", k);
            if (colon == std::string::npos) return "";
            size_t lineEnd = obj.find("\n", colon);
            std::string val = obj.substr(colon+1, lineEnd - (colon+1));
            // trim
            while (!val.empty() && isspace(val.front())) val.erase(val.begin());
            while (!val.empty() && isspace(val.back())) val.pop_back();
            // remove trailing comma if present (fields in save() end with a comma)
            if (!val.empty() && val.back() == ',') { val.pop_back(); while (!val.empty() && isspace(val.back())) val.pop_back(); }
            // if it's an array or string, return as-is
            return val;
        };

        std::string title = getField("title");
        std::string description = getField("description");
        std::string start = getField("start");
        std::string end = getField("end");
        std::string tagsraw = getField("tags");
        std::string rec = getField("recurrence");

        title = unquote(title);
        description = unquote(description);
        start = unquote(start);
        end = unquote(end);

        // Accept files where dates are stored in DD-MM-YYYY HH:MM; convert to YYYY-MM-DD HH:MM
        auto convertDmyToYmdIfNeeded = [](const std::string &s)->std::string{
            if (s.size() >= 16 && isdigit((unsigned char)s[0]) && isdigit((unsigned char)s[1]) && s[2]=='-' && isdigit((unsigned char)s[3]) && isdigit((unsigned char)s[4]) && s[5]=='-' && isdigit((unsigned char)s[6]) ) {
                // pattern DD-MM-YYYY ...
                // ensure positions: 0-1 dd, 3-4 mm, 6-9 yyyy
                std::string dd = s.substr(0,2);
                std::string mm = s.substr(3,2);
                std::string yyyy = s.substr(6,4);
                std::string rest = "";
                if (s.size() > 10) rest = s.substr(10); // from space onwards
                return yyyy + std::string("-") + mm + std::string("-") + dd + rest;
            }
            return s;
        };

        start = convertDmyToYmdIfNeeded(start);
        end = convertDmyToYmdIfNeeded(end);

        auto startTp = TimeUtils::fromString(start);
        auto endTp = TimeUtils::fromString(end);
        auto ev = std::make_shared<Event>(title, startTp, endTp);
        ev->setDescription(description);

        // parse tagslike ["a","b"]
        if (!tagsraw.empty()) {
            size_t p = tagsraw.find('[');
            size_t q = tagsraw.find(']');
            if (p != std::string::npos && q != std::string::npos && q > p) {
                std::string inner = tagsraw.substr(p+1, q - (p+1));
                std::istringstream ss(inner);
                std::string token;
                while (std::getline(ss, token, ',')) {
                    // trim
                    while (!token.empty() && isspace(token.front())) token.erase(token.begin());
                    while (!token.empty() && isspace(token.back())) token.pop_back();
                    token = unquote(token);
                    if (!token.empty()) ev->addTag(token);
                }
            }
        }

        if (!rec.empty()) {
            rec = unquote(rec);
            // Only support "Daily every N day(s)" if present
            if (rec.find("Daily") != std::string::npos) {
                // find number inside
                size_t posN = rec.find("every");
                int n = 1;
                if (posN != std::string::npos) {
                    std::string rest = rec.substr(posN);
                    for (char c : rest) if (isdigit(c)) { n = rest[posN+6] - '0'; break; }
                }
                ev->setRecurrence(std::make_shared<RecurrenceRuleDaily>(n));
            }
        }

        events.push_back(ev);
        pos = objEnd + 1;
    }
    return events;
}

