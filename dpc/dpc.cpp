#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

enum
{
    winlog = 14,
    winsize = (1 << winlog),
    winmask = (1 << winlog) - 1
};

std::vector<std::uint8_t> unLZ(const std::vector<std::uint8_t>& data)
{
    std::uint8_t win[winsize];
    std::uint32_t c, d, r, l, i, j, flag, flagmask, flagbit, pos, lenbits = 2;


    const std::uint8_t* dp = data.data() + 28;

    std::vector<std::uint8_t> decompressed;

    for (i = 0; i < winsize; i++)
        win[i] = 0;

    l = *(std::uint32_t*)dp;
    dp += sizeof(l);

    std::uint32_t len = *(std::uint32_t*)data.data() + 24;

    flagbit = 0;

    for (pos = 0;;)
    {

        if (flagbit <= 1)
        {
            flagmask = *(dp++) << 24;
            flagmask |= *(dp++) << 16;
            flagmask |= *(dp++) << 8;
            flagmask |= *(dp++);
            flagbit = 32 - 1;
            lenbits = winlog - (flagmask & 3);
        }

        flag = (flagmask >> flagbit) & 1;
        flagbit--;

        c = *(dp++);
        if (dp >= data.data() + len)
            break;

        if (flag == 0)
        {
            // literal

            win[(pos++) & winmask] = c;
            decompressed.push_back(c);
        }
        else
        {
            // match

            d = *(dp++);
            j = (c << 8) + d;

            l = (j >> lenbits) + 3;
            d = (j & ((1 << lenbits) - 1)) + 1;

            for (j = 0; j < l; j++)
            {
                c = win[(pos - d) & winmask];
                win[(pos++) & winmask] = c;
                decompressed.push_back(c);
            }
        }
    }

    return decompressed;
}

void rip(std::filesystem::path dpcPath)
{
    std::ifstream dpc(dpcPath, std::ios::binary);

    std::uint32_t begin = 2129920;
    std::uint32_t i = 0;
    std::uint32_t j = 0;

    std::uint32_t buf;

    std::filesystem::create_directory("rip\\");
    std::filesystem::create_directory("rip\\" + dpcPath.stem().string() + "\\");
    std::filesystem::create_directory("rip\\" + dpcPath.stem().string() + "\\images\\");
    std::filesystem::create_directory("rip\\" + dpcPath.stem().string() + "\\data\\");

    const char dxt1512header[] = { '\x44', '\x44', '\x53', '\x20', '\x7C', '\x00', '\x00', '\x00', '\x07', '\x10', '\x02', '\x00', '\x00', '\x02', '\x00', '\x00', '\x00', '\x02', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x0A', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x20', '\x00', '\x00', '\x00', '\x04', '\x00', '\x00', '\x00', '\x44', '\x58', '\x54', '\x31', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x08', '\x10', '\x40', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' };

    const char dxt5256header[] = { '\x44', '\x44', '\x53', '\x20', '\x7C', '\x00', '\x00', '\x00', '\x07', '\x10', '\x02', '\x00', '\x00', '\x01', '\x00', '\x00', '\x00', '\x01', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x09', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x20', '\x00', '\x00', '\x00', '\x04', '\x00', '\x00', '\x00', '\x44', '\x58', '\x54', '\x35', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x08', '\x10', '\x40', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' };

    const char dxt5512header[] = { '\x44', '\x44', '\x53', '\x20', '\x7C', '\x00', '\x00', '\x00', '\x07', '\x10', '\x02', '\x00', '\x00', '\x02', '\x00', '\x00', '\x00', '\x02', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x0A', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x20', '\x00', '\x00', '\x00', '\x04', '\x00', '\x00', '\x00', '\x44', '\x58', '\x54', '\x35', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x08', '\x10', '\x40', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' };

    while (!dpc.eof())
    {
        dpc.seekg(j * 0x800 + 2129920 - sizeof(buf));
        dpc.read((char*)&buf, sizeof(buf));

        if (dpc.eof()) return;

        if (buf == std::numeric_limits<std::uint32_t>::max())
        {
            std::vector<std::uint8_t> data;
            data.resize((std::uint32_t)dpc.tellg() - begin);
            dpc.seekg(begin);
            dpc.read((char*)data.data(), data.size());

            if (*(std::uint32_t*)(data.data()) == 0x0002AAB8 && *(std::uint32_t*)(data.data() + 16) == 0x57B1F99E) // DXT1 512
            {
                if (data.size() != 176128)
                {
                    j++;
                    continue;
                }

                std::ofstream file("rip\\" + dpcPath.stem().string() + "\\images\\" + dpcPath.stem().string() + "_" + std::to_string(i) + "_" + std::to_string(begin) + ".dds", std::ios::binary);
                file.write(dxt1512header, sizeof(dxt1512header));
                file.write((char*)data.data() + 24, data.size() - 24);
            }
            else if (*(std::uint32_t*)(data.data()) == 0x00015570 && *(std::uint32_t*)(data.data() + 16) == 0x57B1F99E) // DXT5 256
            {
                if (data.size() != 88064)
                {
                    j++;
                    continue;
                }

                std::ofstream file("rip\\" + dpcPath.stem().string() + "\\images\\" + dpcPath.stem().string() + "_" + std::to_string(i) + "_" + std::to_string(begin) + ".dds", std::ios::binary);
                file.write(dxt5256header, sizeof(dxt5256header));
                file.write((char*)data.data() + 24, data.size() - 24);
            }
            else if (*(std::uint32_t*)(data.data()) == 0x00055570 && *(std::uint32_t*)(data.data() + 16) == 0x57B1F99E) // DXT5 512
            {
                std::ofstream file("rip\\" + dpcPath.stem().string() + "\\images\\" + dpcPath.stem().string() + "_" + std::to_string(i) + "_" + std::to_string(begin) + ".dds", std::ios::binary);
                file.write(dxt5512header, sizeof(dxt5512header));
                file.write((char*)data.data() + 24, data.size() - 24);
            }
            else if (*(std::uint32_t*)(data.data() + 16) == 0x57B1F99E) // compressed image
            {
                std::ofstream file("rip\\" + dpcPath.stem().string() + "\\images\\" + dpcPath.stem().string() + "_" + std::to_string(i) + "_" + std::to_string(begin) + ".dds", std::ios::binary);

                std::vector<std::uint8_t> decompressed = unLZ(data);
                if (decompressed.size() >= 349679)
                {
                    file.write(dxt5512header, sizeof(dxt5512header));
                }
                else if (decompressed.size() >= 174903)
                {
                    file.write(dxt1512header, sizeof(dxt1512header));
                }
                else if (decompressed.size() >= 87527)
                {
                    file.write(dxt1512header, sizeof(dxt1512header));
                }
                else if (decompressed.size() >= 43831)
                {
                    file.write(dxt1512header, sizeof(dxt1512header));
                }
                else if (decompressed.size() >= 43831)
                {
                    file.write(dxt5256header, sizeof(dxt5256header));
                }
                file.write((char*)decompressed.data(), decompressed.size());
            }
            else if (*(std::uint32_t*)(data.data() + 16) == 0x52B12EB5) // model
            {
                std::ofstream file("rip\\" + dpcPath.stem().string() + "\\data\\" + dpcPath.stem().string() + "_" + std::to_string(i) + "_" + std::to_string(begin) + ".TMESH", std::ios::binary);

                std::vector<std::uint8_t> decompressed = unLZ(data);
                file.write((char*)decompressed.data(), decompressed.size());
            }
            else
            {
                std::ofstream file("rip\\" + dpcPath.stem().string() + "\\" + dpcPath.stem().string() + "_" + std::to_string(i) + "_" + std::to_string(begin) + ".dat", std::ios::binary);
                file.write((char*)data.data(), data.size());
            }

            std::cout << i << " : " << begin << "\n";

            begin = dpc.tellg();
            i++;
        }
        j++;
    }
}

int main()
{
    for (const auto& file : std::filesystem::recursive_directory_iterator("."))
    {
        if (file.path().extension() == ".DPC")
        {
            rip(file.path());
        }
    }
}
