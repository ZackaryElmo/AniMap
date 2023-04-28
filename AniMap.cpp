#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <ratio>
#include <chrono>
#include "AniListTree.h"

using namespace std;
using namespace sf;
using namespace std::chrono;

AniTree::Anime* AniTree::insert(Anime* root, Anime entry)
{
    bool copy = false;

    if (root == nullptr) { // insert node based on string comparisons
        return new Anime(entry);
    }
    else if (entry.romaji_name < root->romaji_name) {
        root->left = insert(root->left, entry);
    }
    else if (entry.romaji_name > root->romaji_name) {
        root->right = insert(root->right, entry);
    }
    else {
        copy = true;
    }

    if (!copy) { // if anime is copy, ignore. Otherwise, update tree node positions and heights
        root->height = getNewHeight(root);
        if (balanceFactor(root) < -1 || balanceFactor(root) > 1) {
            root = rotation(root, balanceFactor(root));
        }
        root->height = getNewHeight(root);
    }


    return root;
}

void AniTree::inorderPrint(Anime* root)
{
    if (root->left != nullptr) {
        inorderPrint(root->left);
    }

    cout << root->romaji_name << " (" << root->english_name << ")" << endl;

    if (root->right != nullptr) {
        inorderPrint(root->right);
    }
}

int AniTree::balanceFactor(Anime* root)
{
    if (root->left == nullptr) {
        return 0 - root->right->height; // no left child, height is 0 or negative
    }
    else if (root->right == nullptr) {
        return root->left->height; // no right child, height is 0 or positive
    }
    else {
        return root->left->height - root->right->height; // otherwise, find difference between left and right
    }
}

int AniTree::getNewHeight(Anime* root)
{
    if (root->left == nullptr && root->right == nullptr) { // no children, height is 1
        return 1;
    }
    else if (root->left == nullptr) {
        return 1 + getNewHeight(root->right); // no left child, find right child height and increase by one
    }
    else if (root->right == nullptr) {
        return 1 + getNewHeight(root->left); // no right child, find left child height and increase by one
    }
    else {
        int left_height = 1 + getNewHeight(root->left); // get's heights of children and assigns the highest value to current node
        int right_height = 1 + getNewHeight(root->right);

        if (left_height > right_height) {
            return left_height;
        }
        else {
            return right_height;
        }
    }
}

AniTree::Anime* AniTree::rotation(Anime* root, int balance)
{
    Anime* temp{};
    if (balance < 0) { // check if tree is right or left heavy, then check if child is right or left heavy to determine rotation
        temp = root->right;

        if (balanceFactor(root->right) == 1) {
            root->right = temp->left;
            temp->left = root->right->right;
            root->right->right = temp;
            temp = root->right;
        }

        root->right = temp->left;
        temp->left = root;
    }
    else if (balance > 0) {
        temp = root->left;

        if (balanceFactor(root->left) == -1) {
            root->left = temp->right;
            temp->right = root->left->left;
            root->left->left = temp;
            temp = root->left;
        }

        root->left = temp->right;
        temp->right = root;
    }

    temp->left->height = getNewHeight(temp->left);
    temp->right->height = getNewHeight(temp->right);

    return temp;
}

void AniTree::insert(string target, string romaji, string english, string start, string end, string cover, string banner, string format, string status, string episodes, string season, string description, string score, vector<string>& genre)
{
    for (string g : genre) { // checks information to see if anime is of the desired genre
        if (g.find(target) != string::npos) {
            root = insert(root, Anime(romaji, english, start, end, cover, banner, format, status, episodes, season, description, score, genre));
            break;
        }
    }
}

void AniTree::inorderPrint()
{
    if (root == nullptr) {
        return;
    }

    cout << "Anime List:" << endl;
    inorderPrint(root);
}

vector<string> AniTree::bfs(string name) //work on print
{
    vector<string> anime; // same logic as dfs, place anime information into vector and return it
    if (root == nullptr) {
        return anime;
    }

    vector<Anime*> queue;

    Anime* temp = root;
    queue.push_back(temp);
    while (!queue.empty()) {
        if (queue.front()->romaji_name == name) {
            anime.push_back(queue.front()->romaji_name);
            anime.push_back(queue.front()->english_name);

            anime.push_back(queue.front()->starting_time);
            anime.push_back(queue.front()->ending_time);

            anime.push_back(queue.front()->cover_image);
            anime.push_back(queue.front()->banner_image);

            anime.push_back(queue.front()->airing_format);
            anime.push_back(queue.front()->airing_status);

            anime.push_back(queue.front()->episodes);
            anime.push_back(queue.front()->season);

            anime.push_back(queue.front()->description);
            anime.push_back(queue.front()->average_score);

            string genre = "[";

            for (string g : queue.front()->genres) {
                genre += g;
            }

            genre += "]";

            anime.push_back(genre);

            return anime;
        }

        if (queue.front()->left != nullptr) {
            queue.push_back(queue.front()->left);
        }

        if (queue.front()->right != nullptr) {
            queue.push_back(queue.front()->right);
        }

        queue.erase(queue.begin());
    }

    return anime;
}

vector<string> AniTree::dfs(string name)
{
    vector<string> anime; // return vector with strings of anime information
    if (root == nullptr) {
        return anime;
    }

    vector<Anime*> stack;

    Anime* temp = root;
    stack.push_back(temp);
    while (!stack.empty()) {
        if (stack.front()->romaji_name == name) { // if anime is found, enter information to vector
            anime.push_back(stack.front()->romaji_name);
            anime.push_back(stack.front()->english_name);

            anime.push_back(stack.front()->starting_time);
            anime.push_back(stack.front()->ending_time);

            anime.push_back(stack.front()->cover_image);
            anime.push_back(stack.front()->banner_image);

            anime.push_back(stack.front()->airing_format);
            anime.push_back(stack.front()->airing_status);

            anime.push_back(stack.front()->episodes);
            anime.push_back(stack.front()->season);

            anime.push_back(stack.front()->description);
            anime.push_back(stack.front()->average_score);

            string genre = "[";

            for (string g : stack.front()->genres) { // group all genres together into one string
                genre += g;
            }

            genre += "]";

            anime.push_back(genre);

            return anime;
        }

        if (stack.front()->left != nullptr) {
            stack.insert(stack.begin() + 1, stack.front()->left);
        }

        if (stack.front()->right != nullptr) {
            stack.insert(stack.begin() + 2, stack.front()->right);
        }

        stack.erase(stack.begin()); // take out first value in stack
    }

    return anime;
}

AniTree::Anime::Anime(string romaji, string english, string start, string end, string cover, string banner, string format, string status, string episodes, string season, string description, string score, vector<string>& genre)
{
    romaji_name = romaji;
    english_name = english;
    starting_time = start;
    ending_time = end;

    cover_image = cover;
    banner_image = banner;

    airing_format = format;
    airing_status = status;

    this->episodes = episodes;
    this->season = season;

    this->description = description;
    average_score = score;
    genres = genre;

    left = nullptr;
    right = nullptr;
    height = 1;
}

void AniTree::clear(Anime* root)
{
    if (root->left != nullptr) {
        clear(root->left);
    }

    if (root->right != nullptr) {
        clear(root->right);
    }

    delete root;
}

void AniTree::clear()
{
    if (root == nullptr) {
        return;
    }

    if (root->left != nullptr) { //post-order traversal for both children
        clear(root->left);
    }

    if (root->right != nullptr) {
        clear(root->right);
    }

    delete root; //delete contents, then set to null
    root = nullptr;
}

int main() {

    bool run_once = true;
    ifstream aniLoader;
    

    string csvLine;

    string romanjiName;
    string englishName;
    string startDate;
    string endDate;
    string coverImage;
    string bannerImage;
    string airingFormat;

    string airingString;
    string episodeCount;
    string startSeason;
    string animeDescription;
    string averageScore;

    vector<string> animeGenres;

    AniTree ani;
    

    // create a font to be used later
    sf::Font font;
        // Passes in the file name so that it can displayed correctly
        if (!font.loadFromFile("Lato-Black.ttf")) {
            return -1;
        }
    RenderWindow window(VideoMode(1500, 900), "AniMap");
    
    
    // makes the bottons for BFS and BFS
    RectangleShape buttonRect(Vector2f(100, 50));
    buttonRect.setPosition(100, 550);
    buttonRect.setFillColor(Color::Blue);
    buttonRect.setOutlineThickness(2);
    buttonRect.setOutlineColor(Color::Black);
    
    // makes DFS button
    RectangleShape buttonRect2(Vector2f(100, 50));
    buttonRect2.setPosition(250, 550);
    buttonRect2.setFillColor(Color::Blue);
    buttonRect2.setOutlineThickness(2);
    buttonRect2.setOutlineColor(Color::Black);

    // BFS button Text
    Text buttonText("BFS", font, 20);
    buttonText.setPosition(120, 560);

    // DFS Button Text
    Text buttonText2("DFS", font, 20);
    buttonText2.setPosition(270, 560);

    //  instructions Text
    Text instructions("Please Choose a Traversal method!", font, 20);
    instructions.setPosition(100, 500);

    Text instructions2("Please Type the Name of an Anime!", font, 20);
    instructions2.setPosition(100, 350);

    // button state checkers
    bool buttonClicked = false;
    bool buttonClicked2 = false;

    // Define the text box rectangle
    sf::RectangleShape textBoxRect(sf::Vector2f(300, 50));
    textBoxRect.setFillColor(sf::Color::Blue);
    textBoxRect.setOutlineThickness(2);
    textBoxRect.setOutlineColor(sf::Color::Black);
    textBoxRect.setPosition(100, 400);

    // Define the box that has the Anime information
    sf::RectangleShape animeInfo(sf::Vector2f(800, 870));
    animeInfo.setFillColor(sf::Color::Black);
    animeInfo.setOutlineThickness(2);
    animeInfo.setOutlineColor(sf::Color::White);
    animeInfo.setPosition(600, 10);

    // Define the AnimeInfo text and creates all the information text.
    Text info("Your Anime's Information:", font, 25);
    info.setPosition(850, 50);
    info.setStyle(sf::Text::Bold | sf::Text::Underlined);

    Text nameEnglishText("English Name:", font, 14);
    nameEnglishText.setPosition(620, 120);

    Text nameRomanjiText("Romanji Name:", font, 14);
    nameRomanjiText.setPosition(620, 150);

    Text episodesText("# of Episodes:", font, 14);
    episodesText.setPosition(620, 180);

    Text startingTimeText("Start Date:", font, 14);
    startingTimeText.setPosition(620, 210);

    Text endingTimeText("End Date:", font, 14);
    endingTimeText.setPosition(620, 240);

    Text airingStatusText("Airing Status:", font, 14);
    airingStatusText.setPosition(620, 270);

    Text airingFormatText("Airing Format:", font, 14);
    airingFormatText.setPosition(620, 300);

    Text averageScoreText("Average Score:", font, 14);
    averageScoreText.setPosition(620, 330);

    Text seasonNumberText("Starting Season:", font, 14);
    seasonNumberText.setPosition(620, 360);

    Text genresText("Genres:", font, 14);
    genresText.setPosition(620, 390);

    Text bannerLink("Banner Link:", font, 14);
    bannerLink.setPosition(620, 420);

    Text coverLink("Cover Link", font, 14);
    coverLink.setPosition(620, 450);

    Text DescriptionText("Description:", font, 14);
    DescriptionText.setPosition(620, 480);

    Text executionTime("The Excecution Time of this Traversal in micro-seconds: ", font, 14);
    executionTime.setPosition(620, 830);

    string message = " ";

    Text coverLinkMessage(message, font, 14);
    coverLinkMessage.setPosition(780, 450);

    Text bannerLinkMessage(message, font, 14);
    bannerLinkMessage.setPosition(780, 420);
    // initialize the texts for the dislay
    // run time clock
    string run = "0";
    Text runTimeC(run, font, 14);
    runTimeC.setPosition(1010, 830);

    // name english
    string english_Name = " ";
    Text englishNameC(english_Name, font, 14);
    englishNameC.setPosition(780, 120);

    // Romanji Name
    string romanji_Name = " ";
    Text romanjiNameC(romanji_Name, font, 14);
    romanjiNameC.setPosition(780, 150);

    // Number of episodes
    string episodesNumber = " ";
    Text episodesNumberC(episodesNumber, font, 14);
    episodesNumberC.setPosition(780, 180);

    // Start Date
    string start_Time = " ";
    Text startTimeC(start_Time, font, 14);
    startTimeC.setPosition(780, 210);

    // end Date 
    string end_Time = " ";
    Text endTimeC(end_Time, font, 14);
    endTimeC.setPosition(780, 240);

    //Airing status
    string airing_Status = " ";
    Text airing_StatusC(airing_Status, font, 14);
    airing_StatusC.setPosition(780, 270);

    //Airing format 
    string airing_Format = " ";
    Text airing_FormatC(airing_Format, font, 14);
    airing_FormatC.setPosition(780, 300);

    // Average Score
    string average_Score = " ";
    Text average_ScoreC(average_Score, font, 14);
    average_ScoreC.setPosition(780, 330);

    // # of Seasons 
    string seasons = " ";
    Text seasonsC(seasons, font, 14);
    seasonsC.setPosition(780, 360);
    // Genres
    string genres = " ";
    Text genresC(seasons, font, 14);
    genresC.setPosition(780, 390);

    //Description
    string description_ = " ";
    Text description_C(description_, font, 16);
    description_C.setPosition(780, 480);
    string str = "";
    // Define More text
    Text instructions3("Please Choose an Anime Catagory!", font, 20);
    instructions3.setPosition(100, 50);

    //strings for the banner and cover link
    string bannerLinkPrint = " ";
    string coverLinkPrint = " ";

    // Define the text box text
    sf::Text textBoxText("", font, 20);
    textBoxText.setPosition(110, 410);

    bool typing = false;

    // Define the dropdown list options
    string options[] = { "Action", "Comedy", "Drama", "Romance", "Sci-Fi", "Mystery"};
    int numOptions = 6;

    // Define the dropdown list rectangle
    RectangleShape dropdownRect(Vector2f(200, 50));
    dropdownRect.setFillColor(Color::Blue);
    dropdownRect.setOutlineThickness(2);
    dropdownRect.setOutlineColor(Color::Black);
    dropdownRect.setPosition(100, 100);

    // Define the dropdown list text
    Text dropdownText(options[0], font, 20);
    dropdownText.setPosition(120, 110);

    // Define the dropdown list arrow
    CircleShape dropdownArrow(10, 3);
    dropdownArrow.setFillColor(Color::Black);
    dropdownArrow.setPosition(280, 120);

    bool dropdownOpen = false;
    string userChoice = "";

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (dropdownRect.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    dropdownOpen = !dropdownOpen;
                }
                else if (dropdownOpen) {
                    for (int i = 0; i < numOptions; i++) {
                        Text optionText(options[i], font, 20);
                        optionText.setPosition(120, 160 + i * 30);
                        if (optionText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            dropdownText.setString(options[i]);
                            dropdownOpen = false;
                            
                            break;
                        }
                    }
                }
                else if(textBoxRect.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    typing = true;
                }
                else if (buttonRect.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    buttonClicked = true;
                }
                else if (buttonRect2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    buttonClicked2 = true;
                }
                else {
                    dropdownOpen = false;
                    typing = false;
                }
            }
            else if (event.type == sf::Event::TextEntered && typing) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == '\b') { // backspace key
                        std::string textString = textBoxText.getString();
                        if (textString.size() > 0) {
                            textString.erase(textString.size() - 1);
                            textBoxText.setString(textString);
                           
                        }
                    }
                    else {
                        textBoxText.setString(textBoxText.getString() + static_cast<char>(event.text.unicode));
                    }
                }
            }
        }

        window.clear(Color::Black);

        // Draw the dropdown list rectangle
        window.draw(dropdownRect);

        // Draw the dropdown list text
        window.draw(dropdownText);

        // Draw the dropdown list arrow
        dropdownArrow.setRotation(dropdownOpen ? 180 : 0);
        window.draw(dropdownArrow);

        // Draw the dropdown list options
        if (dropdownOpen) {
            for (int i = 0; i < numOptions; i++) {
                Text optionText(options[i], font, 20);
                optionText.setPosition(120, 160 + i * 30);
                window.draw(optionText);
            }
        }

        // Draw the text box rectangle
        window.draw(textBoxRect);

        // Draw the text box text
        window.draw(textBoxText);

        // draws the buttons
        window.draw(buttonRect);
        window.draw(buttonText);

        // draws the second button
        window.draw(buttonRect2);
        window.draw(buttonText2);

        // draws the extra text/ directions
        window.draw(instructions);

        window.draw(instructions2);

        window.draw(instructions3);

        // draws the anime info box
        window.draw(animeInfo);

        // draws all the anime information stuff taht is not varible dependent
        window.draw(info);
        window.draw(nameEnglishText);
        window.draw(nameRomanjiText);
        window.draw(episodesText);
        window.draw(startingTimeText);
        window.draw(endingTimeText);
        window.draw(airingStatusText);
        window.draw(airingFormatText);
        window.draw(averageScoreText);
        window.draw(seasonNumberText);
        window.draw(genresText);
        window.draw(DescriptionText);
        window.draw(executionTime);
        window.draw(bannerLink);
        window.draw(coverLink);
        


        
        

        // gets the textbox value
        std::string textBoxValue = textBoxText.getString();
        //std::cout << "Text Box Value: " << textBoxValue << std::endl;

        // Get the dropdown menu value
        userChoice = dropdownText.getString();
        //std::cout << "Text Box Value: " << userChoice << std::endl;
       
        // When BFS is clicked stuff 

        if (buttonClicked) {

            aniLoader.open("ShinAniMap.csv");
            getline(aniLoader, csvLine);
            buttonText.setPosition(120, 560);
            while (getline(aniLoader, csvLine))
            {
                
                string currentColumn;
                istringstream store(csvLine);
                //A backslash is used instead of a comma due to issues with descriptions containing commas and getline. 
                getline(store, currentColumn, '\\');
                romanjiName = currentColumn;

                getline(store, currentColumn, '\\');
                englishName = currentColumn;

                getline(store, currentColumn, '\\');
                startDate = currentColumn;

                getline(store, currentColumn, '\\');
                endDate = currentColumn;

                getline(store, currentColumn, '\\');
                coverImage = currentColumn;

                getline(store, currentColumn, '\\');
                bannerImage = currentColumn;

                getline(store, currentColumn, '\\');
                airingFormat = currentColumn;

                getline(store, currentColumn, '\\');
                airingString = currentColumn;

                getline(store, currentColumn, '\\');
                episodeCount = currentColumn;


                getline(store, currentColumn, '\\');
                startSeason = currentColumn;

                getline(store, currentColumn, '\\');
                animeDescription = currentColumn;

                getline(store, currentColumn, '\\');
                averageScore = currentColumn;

                getline(store, currentColumn, '\\');
                animeGenres.push_back(currentColumn);

                ani.insert(userChoice, romanjiName, englishName, startDate, endDate, coverImage, bannerImage, airingFormat, airingString, episodeCount, startSeason, animeDescription, averageScore, animeGenres);
                animeGenres.clear();
            }
            ani.inorderPrint();
            auto start = high_resolution_clock::now();
            vector<string> output;
            output = ani.bfs(textBoxValue);
            if (!output.empty()) {
                auto finish = high_resolution_clock::now();

                auto runTime = duration_cast<std::chrono::microseconds>(finish - start);

                // update the run varible
                run = to_string(runTime.count());
                runTimeC.setString(run);

                english_Name = output[1];
                englishNameC.setString(english_Name);

                romanji_Name = output[0];
                romanjiNameC.setString(romanji_Name);

                episodesNumber = output[8];
                episodesNumberC.setString(episodesNumber);

                start_Time = output[2];
                startTimeC.setString(start_Time);

                end_Time = output[3];
                endTimeC.setString(end_Time);

                airing_Status = output[7];
                airing_StatusC.setString(airing_Status);

                airing_Format = output[6];
                airing_FormatC.setString(airing_Format);

                average_Score = output[11];
                average_ScoreC.setString(average_Score);

                seasons = output[9];
                seasonsC.setString(seasons);

                genres = output[12];
                genresC.setString(genres);

                message = "Check Terminal";
                coverLinkMessage.setString(message);
                bannerLinkMessage.setString(message);

                description_ = output[10];
                description_C.setString(description_);
                str = description_C.getString();

                bannerLinkPrint = output[5];
                coverLinkPrint = output[4];

                cout << "Banner Image Link: " << bannerLinkPrint << endl;
                cout << "Cover Image Link: " << coverLinkPrint << endl;


                ani.clear();
                aniLoader.close();
            }
            else {
                continue;
            }
           
        }
        
        if(buttonClicked2){
            aniLoader.open("ShinAniMap.csv");
            getline(aniLoader, csvLine);
            buttonText.setPosition(120, 560);
            while (getline(aniLoader, csvLine))
            {

                string currentColumn;
                istringstream store(csvLine);
                //A backslash is used instead of a comma due to issues with descriptions containing commas and getline. 
                getline(store, currentColumn, '\\');
                romanjiName = currentColumn;

                getline(store, currentColumn, '\\');
                englishName = currentColumn;

                getline(store, currentColumn, '\\');
                startDate = currentColumn;

                getline(store, currentColumn, '\\');
                endDate = currentColumn;

                getline(store, currentColumn, '\\');
                coverImage = currentColumn;

                getline(store, currentColumn, '\\');
                bannerImage = currentColumn;

                getline(store, currentColumn, '\\');
                airingFormat = currentColumn;

                getline(store, currentColumn, '\\');
                airingString = currentColumn;

                getline(store, currentColumn, '\\');
                episodeCount = currentColumn;

                getline(store, currentColumn, '\\');
                startSeason = currentColumn;

                getline(store, currentColumn, '\\');
                animeDescription = currentColumn;

                getline(store, currentColumn, '\\');
                averageScore = currentColumn;

                getline(store, currentColumn, '\\');
                animeGenres.push_back(currentColumn);
                
                ani.insert(userChoice, romanjiName, englishName, startDate, endDate, coverImage, bannerImage, airingFormat, airingString, episodeCount, startSeason, animeDescription, averageScore, animeGenres);
                //Clears the genre vector at the end of insertion to save performance.
                animeGenres.clear();
            }
            ani.inorderPrint();
            auto start = high_resolution_clock::now();
            vector<string> output;
            output = ani.dfs(textBoxValue);
            if (!output.empty()) {
                auto finish = high_resolution_clock::now();

                auto runTime = duration_cast<std::chrono::microseconds>(finish - start);
                aniLoader.close();
                // update the run varible
                run = to_string(runTime.count());
                runTimeC.setString(run);

                english_Name = output[1];
                englishNameC.setString(english_Name);

                romanji_Name = output[0];
                romanjiNameC.setString(romanji_Name);

                episodesNumber = output[8];
                episodesNumberC.setString(episodesNumber);

                start_Time = output[2];
                startTimeC.setString(start_Time);

                end_Time = output[3];
                endTimeC.setString(end_Time);

                airing_Status = output[7];
                airing_StatusC.setString(airing_Status);

                airing_Format = output[6];
                airing_FormatC.setString(airing_Format);

                average_Score = output[11];
                average_ScoreC.setString(average_Score);

                seasons = output[9];
                seasonsC.setString(seasons);

                genres = output[12];
                genresC.setString(genres);

                message = "Check Terminal";
                coverLinkMessage.setString(message);
                bannerLinkMessage.setString(message);


                description_ = output[10];
                description_C.setString(description_);
                str = description_C.getString();

                bannerLinkPrint = output[5];
                coverLinkPrint = output[4];

                cout << "Banner Image Link: " << bannerLinkPrint << endl;
                cout << "Cover Image Link: " << coverLinkPrint << endl;

                ani.clear();
            }
            else {
                continue;
            }
            
            
        }

        std::string line = "";

        sf::FloatRect textRect = description_C.getLocalBounds();
        float textWidth = textRect.width;
        float textHeight = textRect.height;

        float lineHeight = description_C.getCharacterSize() * 1.5f;
        float x = description_C.getPosition().x;
        float y = animeInfo.getPosition().y;
        

        for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
            line += *it;
            description_C.setString(line);
            textRect = description_C.getLocalBounds();
            textWidth = textRect.width + 20;

            if (textWidth >= (animeInfo.getSize().x - 180)) {
                line.erase(line.end() - 1);
                description_C.setString(line);
                description_C.setPosition(x, y + 470);
                y += lineHeight;
                line = "";
                line += *it;
            }
            description_C.setString(line);
            description_C.setPosition(x, y + 470);
            window.draw(description_C);
        }

        buttonClicked = false;
        buttonClicked2 = false;
        window.draw(genresC);
        window.draw(coverLinkMessage);
        window.draw(bannerLinkMessage);
        window.draw(seasonsC);
        window.draw(average_ScoreC);
        window.draw(airing_FormatC);
        window.draw(airing_StatusC);
        window.draw(endTimeC);
        window.draw(startTimeC);
        window.draw(episodesNumberC);
        window.draw(romanjiNameC);
        window.draw(runTimeC);
        window.draw(englishNameC);
        window.display();
    }

    return 0;

}
