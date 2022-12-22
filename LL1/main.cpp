#include<bits/stdc++.h>
using namespace std;

void getInput(vector<pair<char,string>> &grammer) {
    string rule;
    int numberOfRules;
    cout << "input the number of rules: ";
    cin >> numberOfRules;
    cout << "note null is $\n";
    cout << "enter rules in format of A->xyz:\n";
    for (int i = 0; i < numberOfRules; i++) {
        cin >> rule;
        if (rule.size() < 4 || rule[0] < 'A' || rule[0] > 'Z' || rule[1] != '-' || rule[2] != '>') {
            cout << "Ooooooohh error in rule\n"
                    "the format must be like \n"
                    "A->bxy try again:\n";
            i--;
        } else {
            ///cout << rule[0] << " " << rule.substr(3) << "\n";
            grammer.push_back({rule[0], rule.substr(3)});
        }
    }
}

void getNullableRulesAndNonTerminals(vector<pair<char,string>> &grammer ,vector<int> &nullRules,set<char> &nullNonTerminals) {
    int cnt = 0;
    char lefSide;
    string rightSide;
    for (auto rule: grammer) {
        tie(lefSide, rightSide) = rule;
        if (rightSide.size() == 1 && rightSide[0] == '$') {
            nullRules.push_back(cnt+1);
            nullNonTerminals.insert(lefSide);
        } else {
            int n = rightSide.size();
            bool isnull = 1;
            for (int i = 0; i < n; i++) {
                if (nullNonTerminals.find(rightSide[i]) == nullNonTerminals.end()) {
                    isnull = 0;
                    break;
                }
            }
            if (isnull) {
                nullRules.push_back(cnt+1);
                nullNonTerminals.insert(lefSide);
            }
        }
        cnt++;
    }
}

void getBWD(vector<pair<char,string>> &grammer,set<char> &nullNonTerminals,map<char,vector<char>> &BDW) {
    char lefSide;
    string rightSide;
    set<pair<char,char>>initBDW;
    for (auto rule: grammer) {
        tie(lefSide, rightSide)=rule;
        if (rightSide.size() == 1 && rightSide[0] == '$') continue;
        for (auto token: rightSide) {
            if (nullNonTerminals.find(token) != nullNonTerminals.end()) {
                initBDW.insert({lefSide, token});
            } else {
                initBDW.insert({lefSide, token});
                break;
            }
        }
    }

    for(auto bwd:initBDW) {
        BDW[bwd.first].push_back(bwd.second);
    }
}

void getAllUniqueTerminalsAndNonTerminals(vector<pair<char,string>> &grammer,set<char> &allUniqueTerminalsAndNonTerminals) {

    char lefSide;
    string rightSide;
    for (auto rule: grammer) {
        tie(lefSide, rightSide) = rule;
        if (rightSide.size() == 1 && rightSide[0] == '$') {
            allUniqueTerminalsAndNonTerminals.insert(lefSide);
            continue;
        }
        allUniqueTerminalsAndNonTerminals.insert(lefSide);
        for (auto token: rightSide) {
            allUniqueTerminalsAndNonTerminals.insert(token);
        }
    }
}

void gettransitive(pair<char,char> start,map<char,vector<char>> &BDW,map<pair<char,char>,bool> &visRules,vector<char> &transitive) {

    visRules[start] = 1;
    transitive.push_back(start.second);

    if (BDW.count(start.second));
    for (auto el: BDW[start.second]) {
        gettransitive({start.second, el}, BDW, visRules, transitive);
    }

}

void getBW(vector<pair<char,string>> &grammer,map<char,vector<char>> &BDW,map<char,vector<char>> &BW) {

    set<pair<char, char>> initBW;
    set<char> allUniqueTerminalsAndNonTerminals;

    //get all unique to calc reflexive
    getAllUniqueTerminalsAndNonTerminals(grammer, allUniqueTerminalsAndNonTerminals);
    for (auto el: allUniqueTerminalsAndNonTerminals) {
        initBW.insert({el, el});
    }

    //remider
    int cnt = 0;
    for (auto bdw: BDW) {
        map<pair<char, char>, bool> visRules;
        vector<char> transitive;
        for (auto el: bdw.second) {
            gettransitive({bdw.first, el}, BDW, visRules, transitive);
        }
        for (auto el: transitive) {
            initBW.insert({bdw.first, el});
        }
    }


    for (auto bw: initBW) {
        BW[bw.first].push_back(bw.second);
    }
}

void getFirstOfAllSymbols(map<char,vector<char>> &BW,map<char,vector<char>> &firstForAll) {

    set<pair<char, char>> initFirst;

    for (auto bw: BW) {
        for (auto el: bw.second) {
            if (el >= 'a' && el <= 'z') {
                initFirst.insert({bw.first, el});
            }
        }
    }

    for (auto first: initFirst) {
        firstForAll[first.first].push_back(first.second);
    }

}

void getFirstForRightSide(vector<pair<char,string>> &grammer,map<char,vector<char>> &firstForAll,set<char> &nullNonTerminals,map<string ,vector<char>> &firstForRightSide) {

    char lefSide;
    string rightSide;
    set<pair<string, char>> initFirstRight;
    for (auto rule: grammer) {
        tie(lefSide, rightSide) = rule;
        char prev = '@';
        for (auto el: rightSide) {
            if (prev == '@') {
                if (el == '$') initFirstRight.insert({rightSide, '$'});
                else {
                    for (auto ell: firstForAll[el]) {
                        initFirstRight.insert({rightSide, ell});
                    }
                }
            } else if (nullNonTerminals.find(prev) != nullNonTerminals.end()) {
                for (auto ell: firstForAll[el]) {
                    initFirstRight.insert({rightSide, ell});
                }
            } else {
                break;
            }
            prev = el;
        }
    }

    for (auto first: initFirstRight) {
        firstForRightSide[first.first].push_back(first.second);
    }

}

void getFDB(vector<pair<char,string>> &grammer,set<char> &nullNonTerminals,map<char,vector<char>> &FDB) {

    char lefSide;
    string rightSide;
    set<pair<char, char>> initFDB;
    for (auto rule: grammer) {
        tie(lefSide, rightSide) = rule;
        int lenOfRight = rightSide.size();
        for (int i = 0; i < lenOfRight; i++) {
            if (rightSide[i] >= 'A' && rightSide[i] <= 'Z') {
                for (int j = i + 1; j < lenOfRight; j++) {
                    if (rightSide[i] >= 'A' && rightSide[i] <= 'Z') {
                        initFDB.insert({rightSide[i], rightSide[j]});
                        if (nullNonTerminals.find(rightSide[j]) == nullNonTerminals.end()) {
                            break;
                        }
                    }
                }
            }
        }
    }

    for (auto fdb: initFDB) {
        FDB[fdb.first].push_back(fdb.second);
    }
}

void getDEO(vector<pair<char,string>> &grammer,set<char> &nullNonTerminals,map<char,vector<char>> &DEO){
    char lefSide;
    string rightSide;
    set<pair<char, char>> initDEO;
    for (auto rule: grammer) {
        tie(lefSide, rightSide) = rule;
        if(rightSide.size()==1 && rightSide[0]=='$')continue;
        int lenOfRightSide = (int)rightSide.size()-1;
        int cnt=0;
        for(;lenOfRightSide>=0;lenOfRightSide--) {
            if(cnt==0) {
                initDEO.insert({rightSide[lenOfRightSide],lefSide});
            }else {
                if(nullNonTerminals.find(rightSide[lenOfRightSide])!=nullNonTerminals.end()){
                    initDEO.insert({rightSide[lenOfRightSide],lefSide});
                } else {
                    if(!(rightSide[lenOfRightSide]>='A' && rightSide[lenOfRightSide]<='Z'))
                        initDEO.insert({rightSide[lenOfRightSide],lefSide});
                    break;
                }
            }
            cnt++;
        }
    }

    for (auto deo: initDEO) {
        DEO[deo.first].push_back(deo.second);
    }
}

void getEO(vector<pair<char,string>> &grammer,set<char> &nullNonTerminals,map<char,vector<char>> &DEO,map<char,vector<char>> &EO) {
    set<pair<char, char>> initEO;
    set<char> allUniqueTerminalsAndNonTerminals;

    //get all unique to calc reflexive
    getAllUniqueTerminalsAndNonTerminals(grammer, allUniqueTerminalsAndNonTerminals);
    // add reflexive without nullableNonTerminals
    for (auto el: allUniqueTerminalsAndNonTerminals) {
        if(nullNonTerminals.find(el)==nullNonTerminals.end())
            initEO.insert({el, el});
    }

    //from DEO
    for(auto deo:DEO) {
        for(auto el:deo.second) {
            initEO.insert({deo.first,el});
        }
    }

    for (auto eo: initEO) {
        EO[eo.first].push_back(eo.second);
    }

}

void getEndOfEOAndBeginOfFDBMatch(char beginOfFDB,map<char,vector<char>> &EO,set<char> &beginOFEO) {

    for (auto eo: EO) {
        for (auto endOfEO: eo.second) {
            if (endOfEO == beginOfFDB) {
                beginOFEO.insert(eo.first);
                break;
            }
        }
    }
}

void getEndOfFDWAndBeginOfDWBMatch(char endOfFDB,map<char,vector<char>> &BW,set<char> &endOfBW) {

    for(auto beginOfBW:BW) {
        if(beginOfBW.first == endOfFDB) {
            for (auto el: beginOfBW.second) {
                endOfBW.insert(el);
            }
        }
    }
}

void getFB( map<char,vector<char>> &EO,map<char,vector<char>> &FDB,map<char, vector<char>> &BW,map<char,vector<char>> &FB){

    set<pair<char, char>> initFB;
    for(auto fdb:FDB) {
        // beginOFEO -> that the end of EO and begin of FDB march
        set<char> beginOFEO;
        getEndOfEOAndBeginOfFDBMatch(fdb.first,EO,beginOFEO);
        // endOfBW -> that the end of FDB and begin of DW march
        for(auto el:fdb.second) {
            set<char> endOfBW;
            getEndOfFDWAndBeginOfDWBMatch(el,BW,endOfBW);

            for(auto begin:beginOFEO){
                for(auto end:endOfBW){
                    initFB.insert({begin,end});
                }
            }
        }
    }

    for (auto fb: initFB) {
        FB[fb.first].push_back(fb.second);
    }

}

void withEndMarker(map<char,vector<char>> &EO,set<char> &PutEndMarker,set<char> nullNonTerminals) {

    for (auto eo: EO) {
        if ((eo.first >= 'A' && eo.first <= 'Z') &&
            (eo.first == 'S' || nullNonTerminals.find(eo.first) != nullNonTerminals.end())) {
            for (auto el: eo.second) {
                if(el=='S') {
                    PutEndMarker.insert(eo.first);
                    break;
                }
            }
        }
    }

}

void getFBWithEndMarker(map<char,vector<char>> &EO,set<char> &nullNonTerminals,map<char,vector<char>> &FB) {
    set<pair<char, char>> initFB;
    set<char> PutEndMarker;

    withEndMarker(EO,PutEndMarker,nullNonTerminals);

    for(auto endMark:PutEndMarker) {
        initFB.insert({endMark,'<'});
    }

    for (auto fb: initFB) {
        FB[fb.first].push_back(fb.second);
    }
}

void getFol(map<char,vector<char>> &FB,set<char> &nullNonTerminals,map<char,vector<char>> &Fol) {

    for(auto fb:FB) {
        if(nullNonTerminals.find(fb.first)!=nullNonTerminals.end()) {
            for (auto el: fb.second) {
                if(el>='a' && el<='z') {
                    Fol[fb.first].push_back(el);
                }
            }
        }
    }

}

void getSel(vector<pair<char,string>> &grammer,map<char,vector<char>> &Fol,map<string, vector<char>> &firstForRightSide,map<int,vector<char>> &Sel) {

    int cnt = 0;
    for (auto rule: grammer) {

        if (rule.second.size() == 1 && rule.second[0] == '$') {
            Sel[cnt + 1] = Fol[rule.first];
        } else {
            Sel[cnt + 1] = firstForRightSide[rule.second];
        }
        cnt++;
    }
}

bool isLL1Gramer(vector<pair<char,string>> &grammer,map<int,vector<char>> &Sel) {

    map<char,pair<int,set<char>>> checkDisjoint;
    int cnt=0;
    for(auto rule:grammer) {
        checkDisjoint[rule.first].first+=Sel[cnt+1].size();
        for(auto sel:Sel[cnt+1]){
            checkDisjoint[rule.first].second.insert(sel);
        }
        cnt++;
    }

    for(auto check:checkDisjoint) {
        if(check.second.first != check.second.second.size()){
            cout<<"Catch joint in: "<<check.first;
            return false;
        }
    }

    return true;
}

/**
 * some print messages functions
 */

// print vector of int
void printVI(const char *message, vector<int> &vi) {
    cout << message << "\n";
    for (auto x: vi) {
        cout << x << "\n";
    }
}

//print set of char
void printSC(const char *message, set<char> &sc) {
    cout << message << "\n";
    for (auto x: sc) {
        cout << x << "\n";
    }
}

// print map of char of vector
void printMCV(const char *message, const char *type,map<char,vector<char>> &mcv) {
    cout<<message<<"\n";
    for(auto x:mcv) {
        for(auto xx:x.second) {
            cout<<x.first<<" "<<type<<" "<<xx<<"\n";
        }
    }
}

// print map of char of vector special case for first
void printMCVSF(const char *message,const char *key,map<char,vector<char>> &mcvsf) {
    cout << message << "\n";
    for (auto x: mcvsf) {
        cout << key<<"(" << x.first << ") = {";
        int cnt = x.second.size();
        for (auto xx: x.second) {
            cout << xx;
            cnt--;
            if(cnt)cout<<",";
        }
        cout << "}\n";
    }
}

// print map of int of vector special case for Sel
void printMIVSF(const char *message,const char *key,map<int,vector<char>> &mcvsf) {
    cout << message << "\n";
    for (auto x: mcvsf) {
        cout << key<<"(" << x.first << ") = {";
        int cnt = x.second.size();
        for (auto xx: x.second) {
            cout << xx;
            cnt--;
            if(cnt)cout<<",";
        }
        cout << "}\n";
    }
}

// print map of char of vector special case for first right
void printMCVSFR(const char *message,map<string ,vector<char>> &mcvsfr) {
    cout << message << "\n";
    for (auto x: mcvsfr) {
        cout << "First(" << x.first << ") = {";
        int cnt = x.second.size();
        for (auto xx: x.second) {
            cout << xx;
            cnt--;
            if(cnt)cout<<",";
        }
        cout << "}\n";
    }
}

void endl(){
    cout<<"\n";
}

int main() {

    /**
     * my variables
     */
    vector<pair<char, string >> grammer;
    vector<int> nullRules;
    set<char> nullNonTerminals;
    map<char, vector<char>> BDW;
    map<char, vector<char>> BW;
    map<char, vector<char>> firstForAll;
    map<string, vector<char>> firstForRightSide;
    map<char,vector<char>> FDB;
    map<char,vector<char>> DEO;
    map<char,vector<char>> EO;
    map<char,vector<char>> FB;
    map<char,vector<char>> Fol;
    map<int,vector<char>> Sel;
    /**
     * take the input
     */
    /*
   S->ABc
   A->bA
   A->$
   B->c
     */
    getInput(grammer);
    endl();

    /**
     * Step 1. Find all nullable rules and nullable non-terminals
     */
    getNullableRulesAndNonTerminals(grammer, nullRules, nullNonTerminals);
    cout<<"Step 1. Find all nullable rules and nullable non-terminals:\n";
    printVI("null rules number:", nullRules);
    printSC("null non-terminals:", nullNonTerminals);
    endl();

    /**
     * Step 2. Compute the relation Begins Directly With for each non-terminal (BDW)
     */
    getBWD(grammer, nullNonTerminals, BDW);
    printMCV("Step 2. Compute the relation Begins Directly With for each non-terminal (BDW):", "BDW", BDW);
    endl();

    /**
     * Step 3. Compute the relation Begins With (BW)
     */
    getBW(grammer, BDW, BW);
    printMCV("Step 3. Compute the relation Begins With (BW):", "BW", BW);
    endl();

    /**
     * Step 4. Compute the set of terminals First(x) for each symbol x in the grammar.
     */
    getFirstOfAllSymbols(BW, firstForAll);
    printMCVSF("Step 4. Compute the set of terminals First(x) for each symbol x in the grammar:","first", firstForAll);
    endl();

    /**
     * Step 5. Compute First of right side of each rule
     */
    getFirstForRightSide(grammer, firstForAll,nullNonTerminals, firstForRightSide);
    printMCVSFR("Step 5. Compute First of right side of each rule:",firstForRightSide);
    endl();

    /**
     * if no nullable rules skip to step 12
     */
     if(nullRules.empty()) {
         cout<<"skip to step 12:(there is no nullRules)\n";
         goto step12;
     }

    /**
     * Step 6. Compute the relation Is Followed Directly By (FDB)
     */
    getFDB(grammer,nullNonTerminals,FDB);
    printMCV("Step 6. Compute the relation Is Followed Directly By (FDB):","FDB",FDB);
    endl();

    /**
     * Step 7. Compute the relation Is Direct End Of (DEO)
     */
    getDEO(grammer,nullNonTerminals,DEO);
    printMCV("Step 7. Compute the relation Is Direct End Of (DEO):","DEO",DEO);
    endl();

    /**
     * Step 8. Compute the relation Is End Of (EO)
     */
    getEO(grammer,nullNonTerminals,DEO,EO);
    printMCV("Step 8. Compute the relation Is End Of (EO):","EO",EO);
    endl();

    /**
     * Step 9. Compute the relation Is Followed By (FB)
     */
    getFB(EO,FDB,BW,FB);
    printMCV("Step 9. Compute the relation Is Followed By (FB):","FB",FB);
    endl();

    /**
     * Step 10. Extend the FB relation to include endmarker (FB with endmarker)
     */
    getFBWithEndMarker(EO,nullNonTerminals,FB);
    printMCV("Step 10. Extend the FB relation to include endmarker (FB with endmarker):","FB",FB);
    endl();

    /**
     * Step 11. Compute the Follow Set for each nullable non-terminal (fol)
     */
    getFol(FB,nullNonTerminals,Fol);
    printMCVSF("Step 11. Compute the Follow Set for each nullable non-terminal (Fol):","Fol",Fol);
    endl();

    step12:
    /**
     * Step 12. Compute the Selection Set for each rule
     */
    getSel(grammer,Fol,firstForRightSide,Sel);
    printMIVSF("Step 12. Compute the Selection Set for each rule (Sel):","Sel",Sel);
    endl();

    /**
     * is LL1 Grammer or not
     */
    if(isLL1Gramer(grammer,Sel)) {
        cout<<"Yeah, it's LL1 Grammer :)";
    } else {
        cout<<"Oh-No, it's Not LL1 Grammer :(";
    }
    endl();
    endl();

    return 0;
}
