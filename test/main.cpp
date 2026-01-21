#include "jsonc-reflection/reflection.hpp"
#include <filesystem>
#include <fstream>

using namespace jsonc_reflection;

struct ReverseProxySettings {
    Renamed<Annotated<std::string, "Used as the server motd", "Allowed values: Any string without semicolon symbol.">, "server-motd"> motd =
        "An EndGate Reverse Proxy Server";

    Renamed<Annotated<bool, "If true then cheats like commands can be used.", "Allowed values: \"true\" or \"false\"">, "allow-cheats"> allowCheats =
        false;

    Renamed<Annotated<std::string, "Used as the world name displayed", "Allowed values: Any string without semicolon symbol.">, "level-name">
        levelName = "EndGate Server";

    Renamed<Annotated<uint32_t, "The maximum number of players that can play on the server.", "Allowed values: Any positive integer">, "max-players">
        maxPlayers = 20;

    Renamed<
        Annotated<
            bool,
            "If true then all connected players must be authenticated to Xbox Live.",
            "Clients connecting to remote (non-LAN) servers will always require Xbox Live authentication regardless of "
            "this setting.",
            "If the server accepts connections from the Internet, then it's highly recommended to enable online-mode.",
            "Allowed values: \"true\" or \"false\"">,
        "online-mode">
        onlineMode = true;

    Renamed<
        Annotated<
            bool,
            "If true then all connected players must be listed in the separate whitelist.json file.",
            "Allowed values: \"true\" or \"false\"">,
        "white-list">
        whiteList = false;

    Renamed<
        Annotated<Ranged<uint16_t, 1, 65535>, "Which IPv4 port the server should listen to.", "Allowed values: Integers in the range [1, 65535]">,
        "server-port">
        serverPort = (uint16_t)19132;

    Renamed<
        Annotated<Ranged<uint16_t, 1, 65535>, "Which IPv6 port the server should listen to.", "Allowed values: Integers in the range [1, 65535]">,
        "server-portv6">
        serverPortv6 = (uint16_t)19133;

    Renamed<
        Annotated<bool, "Force clients to use texture packs provided by the server", "Allowed values: \"true\" or \"false\"">,
        "texturepack-required">
        texturepackRequired = false;

    Annotated<std::string, "Used as server locale code", "Allowed values: Any locale code string (such as en_US)."> language = "Unknown";

    struct ProtocolCheckOption {
        Renamed<Annotated<bool, "Whether fetch ProtocolLib updates automatically.", "Allowed values: \"true\" or \"false\"">, "auto-update">
            autoUpdate = true;

        Renamed<Annotated<bool, "Whether fetch ProtocolLib pre-release version.", "Allowed values: \"true\" or \"false\"">, "fetch-prelease">
            fetchPrelease = false;

        Renamed<Annotated<std::string, "API host to fetch latest release tag.", "Allowed values: Any URL string">, "api-host"> apiHost =
            "api.github.com";

        Renamed<Annotated<std::string, "host to download latest ProtocolLib.", "Allowed values: Any URL string">, "download-host"> downloadHost =
            "github.com";
    };

    Renamed<
        Annotated<
            ProtocolCheckOption,
            "ProtocolLib download and update options",
            "In general, you don't need to change these options unless GitHub isn't accessible in your region">,
        "protocol">
        protocolLibOptions;

    Renamed<
        Annotated<Ranged<int, 0>, "Maximum number of commands retained in console history.", "Allowed values: Any positive integer">,
        "max-console-history-count">
        maxConsoleHistory = 100;
};

std::optional<std::string> readFile(std::filesystem::path const& filePath, bool isBinary = false) {
    if (!std::filesystem::exists(filePath)) { return std::nullopt; }
    std::ifstream           fRead;
    std::ios_base::openmode mode = std::ios_base::in;
    if (isBinary) mode |= std::ios_base::binary;
    fRead.open(filePath, mode);
    if (!fRead.is_open()) { return std::nullopt; }
    std::string data((std::istreambuf_iterator<char>(fRead)), {});
    fRead.close();
    return data;
}

bool writeFile(std::filesystem::path const& filePath, std::string_view content, bool isBinary = false) {
    std::ofstream           fWrite;
    std::ios_base::openmode mode = std::ios_base::out;
    if (isBinary) mode |= std::ios_base::binary;
    if (!std::filesystem::exists(filePath.parent_path())) { std::filesystem::create_directories(filePath.parent_path()); }
    fWrite.open(filePath, mode);
    if (!fWrite.is_open()) { return false; }
    fWrite << content;
    fWrite.close();
    return true;
}

int main() {
    ReverseProxySettings settings;
    if (auto content = readFile("./test.jsonc"); content) {
        if (auto json = jsonc::parse(*content, true); json) { jsonc_reflection::deserialize(settings, *json).value(); }
    }
    writeFile("./test.jsonc", jsonc_reflection::serialize(settings)->dump());
    return 0;
}