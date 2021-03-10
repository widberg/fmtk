#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

//std::vector<std::vector<std::uint8_t>> objects;

struct MaterialAnim_Z_Data
{
    std::uint32_t unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;
    std::uint32_t unknown3;
    std::uint32_t unknown4;
    std::uint32_t unknown5;
    std::uint32_t unknown6;
    std::uint32_t unknown7;
    std::uint32_t unknown8;
    std::uint32_t unknown9;
    std::uint32_t unknown10;
    std::uint32_t unknown11;
    std::uint32_t unknown12;
    std::uint32_t unknown13;
    std::uint32_t unknown14;
    std::uint32_t materialCRC32;
    std::uint8_t unknown15;
    std::uint8_t unknown16;
    std::uint8_t unknown17;
    std::uint8_t unknown18;
    std::uint8_t unknown19;
};

int main(int argc, const char* argv[])
{


    return 0;

	//std::ifstream in("C:\\Users\\jared\\Desktop\\New folder\\RandBIKE.DPC", std::ios::binary);
	//if (!in.good())
	//{
	//	return 1;
	//}

	//std::uint32_t size;

	//while (!in.read(reinterpret_cast<char*>(&size), sizeof(size)).eof())
	//{
	//	std::vector<std::uint8_t> data;
	//	data.resize(size + 24);
	//	*reinterpret_cast<std::uint32_t*>(data.data()) = size;
	//	in.read(reinterpret_cast<char*>(data.data() + 4), data.size() - 4);
	//	objects.push_back(data);
	//}

	//std::ofstream out("C:\\Users\\jared\\Desktop\\New folder\\OutRandBIKE.DPC", std::ios::binary);

	//for (auto it = objects.rbegin(); it != objects.rend(); ++it)
	//{
	//	out.write(reinterpret_cast<char*>((*it).data()), (*it).size());
	//}

    //MaterialAnim_Z_Data min;
    //MaterialAnim_Z_Data max;

    //for (auto file : std::filesystem::directory_iterator("C:\\Users\\jared\\Desktop\\New folder\\New folder"))
    //{
    //    std::ifstream in(file.path(), std::ios::binary);
    //    MaterialAnim_Z_Data materialAnim_Z_Data;
    //    in.read(reinterpret_cast<char*>(&materialAnim_Z_Data), sizeof(materialAnim_Z_Data));

    //    min.unknown0 = std::min(min.unknown0, materialAnim_Z_Data.unknown0);
    //    min.unknown1 = std::min(min.unknown1, materialAnim_Z_Data.unknown1);
    //    min.unknown2 = std::min(min.unknown2, materialAnim_Z_Data.unknown2);
    //    min.unknown3 = std::min(min.unknown3, materialAnim_Z_Data.unknown3);
    //    min.unknown4 = std::min(min.unknown4, materialAnim_Z_Data.unknown4);
    //    min.unknown5 = std::min(min.unknown5, materialAnim_Z_Data.unknown5);
    //    min.unknown6 = std::min(min.unknown6, materialAnim_Z_Data.unknown6);
    //    min.unknown7 = std::min(min.unknown7, materialAnim_Z_Data.unknown7);
    //    min.unknown8 = std::min(min.unknown8, materialAnim_Z_Data.unknown8);
    //    min.unknown9 = std::min(min.unknown9, materialAnim_Z_Data.unknown9);
    //    min.unknown10 = std::min(min.unknown10, materialAnim_Z_Data.unknown10);
    //    min.unknown11 = std::min(min.unknown11, materialAnim_Z_Data.unknown11);
    //    min.unknown12 = std::min(min.unknown12, materialAnim_Z_Data.unknown12);
    //    min.unknown13 = std::min(min.unknown13, materialAnim_Z_Data.unknown13);
    //    min.unknown14 = std::min(min.unknown14, materialAnim_Z_Data.unknown14);
    //    min.materialCRC32 = std::min(min.materialCRC32, materialAnim_Z_Data.materialCRC32);
    //    min.unknown15 = std::min(min.unknown15, materialAnim_Z_Data.unknown15);
    //    min.unknown16 = std::min(min.unknown16, materialAnim_Z_Data.unknown16);
    //    min.unknown17 = std::min(min.unknown17, materialAnim_Z_Data.unknown17);
    //    min.unknown18 = std::min(min.unknown18, materialAnim_Z_Data.unknown18);
    //    min.unknown19 = std::min(min.unknown19, materialAnim_Z_Data.unknown19);

    //    max.unknown0 = std::max(max.unknown0, materialAnim_Z_Data.unknown0);
    //    max.unknown1 = std::max(max.unknown1, materialAnim_Z_Data.unknown1);
    //    max.unknown2 = std::max(max.unknown2, materialAnim_Z_Data.unknown2);
    //    max.unknown3 = std::max(max.unknown3, materialAnim_Z_Data.unknown3);
    //    max.unknown4 = std::max(max.unknown4, materialAnim_Z_Data.unknown4);
    //    max.unknown5 = std::max(max.unknown5, materialAnim_Z_Data.unknown5);
    //    max.unknown6 = std::max(max.unknown6, materialAnim_Z_Data.unknown6);
    //    max.unknown7 = std::max(max.unknown7, materialAnim_Z_Data.unknown7);
    //    max.unknown8 = std::max(max.unknown8, materialAnim_Z_Data.unknown8);
    //    max.unknown9 = std::max(max.unknown9, materialAnim_Z_Data.unknown9);
    //    max.unknown10 = std::max(max.unknown10, materialAnim_Z_Data.unknown10);
    //    max.unknown11 = std::max(max.unknown11, materialAnim_Z_Data.unknown11);
    //    max.unknown12 = std::max(max.unknown12, materialAnim_Z_Data.unknown12);
    //    max.unknown13 = std::max(max.unknown13, materialAnim_Z_Data.unknown13);
    //    max.unknown14 = std::max(max.unknown14, materialAnim_Z_Data.unknown14);
    //    max.materialCRC32 = std::max(max.materialCRC32, materialAnim_Z_Data.materialCRC32);
    //    max.unknown15 = std::max(max.unknown15, materialAnim_Z_Data.unknown15);
    //    max.unknown16 = std::max(max.unknown16, materialAnim_Z_Data.unknown16);
    //    max.unknown17 = std::max(max.unknown17, materialAnim_Z_Data.unknown17);
    //    max.unknown18 = std::max(max.unknown18, materialAnim_Z_Data.unknown18);
    //    max.unknown19 = std::max(max.unknown19, materialAnim_Z_Data.unknown19);
    //}

    //std::cout <<
    //    "unknown0: [" << min.unknown0 << ", " << max.unknown0 << "]\n" <<
    //    "unknown1: [" << min.unknown1 << ", " << max.unknown1 << "]\n" <<
    //    "unknown2: [" << min.unknown2 << ", " << max.unknown2 << "]\n" <<
    //    "unknown3: [" << min.unknown3 << ", " << max.unknown3 << "]\n" <<
    //    "unknown4: [" << min.unknown4 << ", " << max.unknown4 << "]\n" <<
    //    "unknown5: [" << min.unknown5 << ", " << max.unknown5 << "]\n" <<
    //    "unknown6: [" << min.unknown6 << ", " << max.unknown6 << "]\n" <<
    //    "unknown7: [" << min.unknown7 << ", " << max.unknown7 << "]\n" <<
    //    "unknown8: [" << min.unknown8 << ", " << max.unknown8 << "]\n" <<
    //    "unknown9: [" << min.unknown9 << ", " << max.unknown9 << "]\n" <<
    //    "unknown10: [" << min.unknown10 << ", " << max.unknown10 << "]\n" <<
    //    "unknown11: [" << min.unknown11 << ", " << max.unknown11 << "]\n" <<
    //    "unknown12: [" << min.unknown12 << ", " << max.unknown12 << "]\n" <<
    //    "unknown13: [" << min.unknown13 << ", " << max.unknown13 << "]\n" <<
    //    "unknown14: [" << min.unknown14 << ", " << max.unknown14 << "]\n" <<
    //    "materialCRC32: [" << min.materialCRC32 << ", " << max.materialCRC32 << "]\n" <<
    //    "unknown15: [" << min.unknown15 << ", " << max.unknown15 << "]\n" <<
    //    "unknown16: [" << min.unknown16 << ", " << max.unknown16 << "]\n" <<
    //    "unknown17: [" << min.unknown17 << ", " << max.unknown17 << "]\n" <<
    //    "unknown18: [" << min.unknown18 << ", " << max.unknown18 << "]\n" <<
    //    "unknown19: [" << min.unknown19 << ", " << max.unknown19 << "]\n";
}
