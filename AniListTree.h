#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class AniTree{
    private:
        struct Anime {
            string romaji_name;
            string english_name;

            string starting_time;
            string ending_time;

            string cover_image;
            string banner_image;

            string airing_format;
            string airing_status;

            string episodes;
            string season;

            string description;
            string average_score;
            vector<string> genres;

            Anime* left;
            Anime* right;
            int height;

            Anime(string romaji, string english, string start, string end, string cover, string banner, string format, string status, string episodes, string season, string description, string score, vector<string>& genre);
        };
        Anime* root = nullptr;
        Anime* insert(Anime* root, Anime entry);
        void inorderPrint(Anime* root);
        int balanceFactor(Anime* root);
        int getNewHeight(Anime* root);
        AniTree::Anime* rotation(Anime* root, int balance);
        void clear(Anime* root);
    public:
        void insert(string target, string romaji, string english, string start, 
        string end, string cover, string banner, string format, 
        string status, string episodes, string season, string description, 
        string score, vector<string>& genre);
        void inorderPrint();
        vector<string> bfs(string name);
        vector<string> dfs(string name);
        void clear();
};
