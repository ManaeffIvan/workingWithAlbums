#include<iostream>
#include<string>
#include<fstream>


using namespace std;

const int SZ_TRACKS = 3;

enum Genre {
    FUNK,
    SOUL,
    ROCK,
    METAL,
    POP,
    JAZZ
};

string getGenre(Genre x) {
    const int SZ = 6;
    string genres[SZ] = { "Funk", "Soul", "Rock", "Metal", "Pop", "Jazz" };
    return genres[x];
}

struct Track {
    string name;

    int duration;
};

struct Album {
    string title;
    string artist;

    Genre style;

    int year;
    int totalDuration;

    Track tracks[SZ_TRACKS];
};

void sval(Album& a, Album& b) {
    Album c = a;
    a = b;
    b = c;
}

void printAlbum(const Album& a) {
    cout << "------------------------------------------" << endl;
    cout << "Album: " << a.title << " | Artist: " << a.artist << endl;
    cout << "Genre: " << getGenre(a.style) << " | Year: " << a.year << endl;
    cout << "Total Duration: " << a.totalDuration << " sec" << endl;
    cout << "Tracks:" << endl;
    for (int i = 0; i < SZ_TRACKS; i++) {
        cout << "  - " << a.tracks[i].name << " (" << a.tracks[i].duration << "s)" << endl;
    }
}

void editAlbum(Album& a, Album& newA) {
    a = newA;
}

void editAlbum(Album& a) {
    Album newA;
    cout << "------------------------------------------";

    cout << "\nPlease enter album title: "; 
    cin >> newA.title;

    cout << "\nPlease enter album artist: ";
    cin >> newA.artist;

    cout << "\nPlease enter album genre: ";
    int genreInt;
    cin >> genreInt;
    newA.style = Genre(genreInt);

    cout << "\nPlease enter album year: ";
    cin >> newA.year;

    cout << "\nPlease enter album title: ";
    cin >> newA.title;

    cout << "\nPlease enter album total duration(sec): ";
    cin >> newA.totalDuration;

    cout << "\nPlease enter album tracks(" << SZ_TRACKS << ")";
    for (int i = 0; i < SZ_TRACKS; i++) {
        cout << "\n\tPlease enter track name:";
        cin >> newA.tracks[i].name;

        cout << "\n\tPlease enter track duration(sec):";
        cin >> newA.tracks[i].duration;
    }

    editAlbum(a, newA);
    cout << "Album changed" << endl;
}

//void printFilteredCatalog(const vector<Album>& v, string msg="") {
//    cout << "\n>>> " << msg << " (Count: " << v.size() << ") <<<" << endl;
//    for (const auto& a: v) {
//        printAlbum(a);
//    }
//}

void printFilteredCatalog(Album* v, int sz, string msg = "") {
    cout << "\n>>> " << msg << " (Count: " << sz << ") <<<" << endl;
    for (int i = 0; i < sz; i++) {
        printAlbum(*(v + i));
    }
}

void myBubbleSort(Album* v, int sz, bool(*comp) (Album, Album)) {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz - 1 - i; j++) {
            if (comp(v[j], v[j + 1])) {
                sval(v[j], v[j + 1]);
            }
        }
    }
}

bool myCompYear(Album a, Album b) {
    return a.year > b.year;
}

void myBubbleSortByYear(Album* v, int sz) {
    myBubbleSort(v, sz, myCompYear);
}

void findTop3Longest(Album* albums, int sz, Album* top3) {
    top3[0] = *albums;
    top3[1] = *(albums + 1);
    top3[2] = *(albums + 2);
    if (top3[0].totalDuration < top3[1].totalDuration) {
        sval(top3[0], top3[1]);
    }
    if (top3[1].totalDuration < top3[2].totalDuration) {
        sval(top3[1], top3[2]);
    }
    if (top3[0].totalDuration < top3[1].totalDuration) {
        sval(top3[0], top3[1]);
    }

    for (int i = 0; i < sz; i++) {
        int j = 0;
        Album cur = *(albums + i);
        for (; j < 3; j++) {
            if (top3[j].totalDuration < cur.totalDuration) {
                break;
            }
        }
        if (j != 3) {
            for (int k = 2; k > j; k--) {
                top3[k] = top3[k - 1];
            }
            top3[j] = cur;
        }
    }
}

void changeYearFile(Album* albums, int sz, string fileName) {
    ifstream fin;
    string title, str;
    int year, ln, i, tenDegree;
    fin.open(fileName);
    if (fin.is_open()) {
        while (!fin.eof())
        {
            getline(fin, str);
            ln = str.length(), year = 0, tenDegree = 1;
            title = "";

            for (i = ln - 1; i > -1 && str[i] >= '0' && str[i] <= '9'; i--) {
                year += tenDegree * (str[i] - '0');
                tenDegree *= 10;
            }

            while (i > -1 && str[i] == ' ') {
                i--;
            }
            title = str.substr(0, i + 1);

            for (int i = 0; i < sz; i++) {
                if ((albums + i)->title == title) {
                    (albums + i)->year = year;
                }
            }
        }
    }
    else {
        cout << "WARNING: File not found\n";
    }
    fin.close();
}

void myInit(Album* albums, int sz) {
    fstream in("binary_input.txt", ios::binary | ios::in);
    if (in.is_open()) {
        Album cur = { "What Is Hippo", "Tower of Power", FUNK, 1973, 2400, {{"Soul Vaccination", 300}, {"What Is Hip", 305}, {"c", 0}} };
        int len;
        for (int i = 0; i < sz && !in.eof(); i++) {
            in.read((char*)&len, sizeof(len));
            cur.title.resize(len);
            in.read(&cur.title[0], len);

            in.read((char*)&len, sizeof(len));
            cur.artist.resize(len);
            in.read(&cur.artist[0], len);

            in.read((char*)&cur.style, sizeof(cur.style));
            in.read((char*)&cur.year, sizeof(cur.year));
            in.read((char*)&cur.totalDuration, sizeof(cur.totalDuration));

            for (int j = 0; j < SZ_TRACKS; j++) {
                in.read((char*)&len, sizeof(len));
                cur.tracks[j].name.resize(len);
                in.read(&cur.tracks[j].name[0], len);

                in.read((char*)&cur.tracks[j].duration, sizeof(cur.tracks[j].duration));
            }

            //in.read((char*)&cur, sizeof(Album));

            * (albums + i) = cur;
        }
        in.close();
    }
    else {
        * (albums + 0) = { "What Is Hippo", "Tower of Power", FUNK, 1973, 2400, {{"Soul Vaccination", 300}, {"What Is Hip", 305}, {"c", 0}} };
        * (albums + 1) = { "Meteora", "Linkin Park", METAL, 2003, 2190, {{"Faint", 162}, {"Numb", 185}, {"c", 0}} };
        * (albums + 2) = { "Soulful Strut", "Young-Holt", SOUL, 1968, 1900, {{"Strut", 180}, {"Track 2", 200}, {"c", 0}} };
        * (albums + 3) = { "Hybrid Theory", "Linkin Park", METAL, 2000, 2252, {{"Papercut", 184}, {"In the End", 216}, {"c", 0}} };
        * (albums + 4) = { "In Step", "SRV", ROCK, 1989, 2450, {{"Crossfire", 250}, {"Riviera", 300}, {"c", 0}} };
        * (albums + 5) = { "Superfly", "Curtis Mayfield", FUNK, 1972, 2100, {{"Pusherman", 304}, {"Superfly", 230}, {"c", 0}} };
        * (albums + 6) = { "Innervisions", "Stevie Wonder", SOUL, 1973, 2650, {{"Higher Ground", 222}, {"Living", 442}, {"c", 0}} };
        * (albums + 7) = { "A Thousand Suns", "Linkin Park", ROCK, 2010, 2800, {{"The Catalyst", 339}, {"Waiting", 230}, {"c", 0}} };
        * (albums + 8) = { "One More Light", "Linkin Park", POP, 2017, 2100, {{"Heavy", 169}, {"Invisible", 210}, {"c", 0}} };
        * (albums + 9) = { "The Payback", "James Brown", FUNK, 1973, 4400, {{"The Payback", 450}, {"Doing It", 550}, {"c", 0}} };
        * (albums + 10) = { "Kind of Blue", "Miles Davis", JAZZ, 1959, 2700, {{"So What", 560}, {"Blue", 700}, {"c", 0}} };
        * (albums + 11) = { "Reanimation", "Linkin Park", METAL, 2002, 3600, {{"Pts.Of.Athrty", 210}, {"Enth E Nd", 230}, {"c", 0}} };
        * (albums + 12) = { "Off the Wall", "Michael Jackson", SOUL, 1979, 2550, {{"Rock with You", 220}, {"Burn", 300}, {"c", 0}} };
        * (albums + 13) = { "Back in Black", "AC/DC", ROCK, 1980, 2520, {{"Hells Bells", 312}, {"Shoot", 330}, {"c", 0}} };
        * (albums + 14) = { "Living Things", "Linkin Park", ROCK, 2012, 2220, {{"Burn It Down", 230}, {"Castle", 205}, {"c", 0}} };
        * (albums + 15) = { "Fresh", "Sly Stone", FUNK, 1973, 2000, {{"In Time", 300}, {"Que Sera", 320}, {"c", 0}} };
        * (albums + 16) = { "The Hunting Party", "Linkin Park", ROCK, 2014, 2700, {{"Guilty", 234}, {"Final", 280}, {"c", 0}} };
        * (albums + 17) = { "What's Going On", "Marvin Gaye", SOUL, 1971, 2100, {{"Mercy", 194}, {"Inner", 300}, {"c", 0}} };
        * (albums + 18) = { "Minutes to Midnight", "Linkin Park", ROCK, 2007, 2600, {{"Given Up", 189}, {"Bleed It Out", 164}, {"c", 0}} };
        * (albums + 19) = { "Maggot Brain", "Funkadelic", FUNK, 1971, 2200, {{"Maggot Brain", 620}, {"Can You", 430}, {"c", 0}} };
    }
}

void save(Album* albums, int sz, string file_name = "binary_input.txt") {
    ofstream out("binary_input.txt", ios::binary | ios::out);
    int len;
    for (int i = 0; i < sz; i++) {
        Album cur = *(albums + i);

        len = cur.title.length();
        out.write((char*)&len, sizeof(len));
        out.write(cur.title.c_str(), len);

        len = cur.artist.length();
        out.write((char*)&len, sizeof(len));
        out.write(cur.artist.c_str(), len);

        out.write((char*)&cur.style, sizeof(cur.style));
        out.write((char*)&cur.year, sizeof(cur.year));
        out.write((char*)&cur.totalDuration, sizeof(cur.totalDuration));

        for (int j = 0; j < SZ_TRACKS; j++) {
            len = cur.tracks[j].name.length();
            out.write((char*)&len, sizeof(len));
            out.write(cur.tracks[j].name.c_str(), len);

            out.write((char*)&cur.tracks[j].duration, sizeof(cur.tracks[j].duration));
        }
        // out.write((char*)&cur, sizeof(Album));
    }
    out.close();
}


int main() {
    setlocale(LC_ALL, "");

    const int N = 20;
    Album catalog[N];
    for (int i = 0; i < N; i++) {
        catalog[i] = { "What Is Hippo", "Tower of Power", FUNK, 1973, 2400, {{"Soul Vaccination", 300}, {"What Is Hip", 305}, {"c", 0}} };

    }
    myInit(catalog, N);

    Album funkSoulAlbums[N];
    int sz1 = 0;
    for (int i = 0; i < N; i++) {
        if (catalog[i].style == FUNK || catalog[i].style == SOUL) {
            funkSoulAlbums[sz1] = catalog[i];
            sz1++;
        }
    }

    myBubbleSortByYear(funkSoulAlbums, sz1);
    printFilteredCatalog(funkSoulAlbums, sz1, "JUAST FUNK AND SOUL ALBUMS");
    cout << "\n\n";

    const int sz3 = 3;
    Album top3[sz3];
    findTop3Longest(catalog, N, top3);
    printFilteredCatalog(top3, sz3, "TOP 3 LONGEST ALBUMS");
    cout << "\n\n";

    printAlbum(catalog[5]);
    cout << "\n\n";

    cout << "Please enter the album number (1.." << N << ") you want to edit: ";
    int num;
    cin >> num;
    num--;
    if (num >= 0 && num <= N - 1) {
        editAlbum(catalog[num]);
        printAlbum(catalog[num]);
    }
    else {
        cout << "INCORRECT NUMBER";
    }
    cout << "\n\n";

    Album lpAlbums[N];
    int sz2 = 0;
    for (int i = 0; i < N; i++) {
        if (catalog[i].artist == "Linkin Park") {
            lpAlbums[sz2] = catalog[i];
            sz2++;
        }
    }
    printFilteredCatalog(lpAlbums, sz2, "ALL LINKIN PARK ALBUMS FROM CATALOG");
    cout << "\n\n";

    const int M = 8;
    Album lpcatalog[M] = {
        {"Meteora", "Linkin Park", METAL, 2003, 2190, {{"Faint", 162}, {"Numb", 185}}},
        {"Hybrid Theory", "Linkin Park", METAL, 2000, 2252, {{"Papercut", 184}, {"In the End", 216}}},
        {"A Thousand Suns", "Linkin Park", ROCK, 2010, 2800, {{"The Catalyst", 339}, {"Waiting", 230}}},
        {"One More Light", "Linkin Park", POP, 2017, 2100, {{"Heavy", 169}, {"Invisible", 210}}},
        {"Reanimation", "Linkin Park", METAL, 2002, 3600, {{"Pts.Of.Athrty", 210}, {"Enth E Nd", 230}}},
        {"Living Things", "Linkin Park", ROCK, 2012, 2220, {{"Burn It Down", 230}, {"Castle", 205}}},
        {"The Hunting Party", "Linkin Park", ROCK, 2014, 2700, {{"Guilty", 234}, {"Final", 280}}},
        {"Minutes to Midnight", "Linkin Park", ROCK, 2007, 2600, {{"Given Up", 189}, {"Bleed It Out", 164}}}
    };
    printFilteredCatalog(lpcatalog, M, "ALL LINKIN PARK ALBUMS");
    cout << "\n\n";

    cout << "\n>>>CHANGING YEAR<<<\n";
    changeYearFile(catalog, N, "newYears.txt");
    printAlbum(catalog[1]);
    printAlbum(catalog[2]);
    cout << "\n\n";

    save(catalog, N);
    printFilteredCatalog(catalog, N);

    return 0;
}