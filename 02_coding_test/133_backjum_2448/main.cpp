#include <iostream>
#include <vector>

using namespace std;

void draw(pair<int, int> idx, int n, vector<vector<char>>& picture);

int main(int argc, char const *argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int n;
    cin >> n;
    vector<vector<char>> picture(n, vector<char>(n * 2 - 1, ' '));

    draw({0, 0}, n, picture);

    for (auto& v : picture) {
        for (char& c : v) {
            cout << c;
        }
        cout << "\n";
    }
    
    return 0;
}

void draw(pair<int, int> idx, int n, vector<vector<char>>& picture) {
    if (n/3 == 1) {
        for (int i = 0; i < 3; i++) {
            int j = 2 - i;
            int t = j + 1 + i + i;

            for(int j = 2 - i; j < t; j++) {
                if (i == 1 && j == 2) { continue; }
                picture[idx.first + i][idx.second + j] = '*';
            }
        }
        return;
    }

    draw({idx.first, idx.second + n/2}, n/2, picture);
    draw({idx.first + n/2, idx.second}, n/2, picture);
    draw({idx.first + n/2, idx.second + n}, n/2, picture);
}

// 0,0
// 3
//   *  
//  * * 
// *****

//   0,6
// 3,0 3,5
// 6
//      *     
//     * *    
//    *****   
//   *     *  
//  * *   * * 
// ***** *****

//   0,6
// 3,6 3,12

// 12
//            *           
//           * *          
//          *****         
//         *     *        
//        * *   * *       
//       ***** *****      
//      *           *     
//     * *         * *    
//    *****       *****   
//   *     *     *     *   
//  * *   * *   * *   * * 
// ***** ***** ***** *****

// 24
//                        *                        
//                       * *                       
//                      *****                      
//                     *     *                     
//                    * *   * *                    
//                   ***** *****                   
//                  *           *                  
//                 * *         * *                 
//                *****       *****                
//               *     *     *     *               
//              * *   * *   * *   * *              
//             ***** ***** ***** *****             
//            *                       *            
//           * *                     * *           
//          *****                   *****          
//         *     *                 *     *         
//        * *   * *               * *   * *        
//       ***** *****             ***** *****       
//      *           *           *           *      
//     * *         * *         * *         * *     
//    *****       *****       *****       *****    
//   *     *     *     *     *     *     *     *   
//  * *   * *   * *   * *   * *   * *   * *   * *  
// ***** ***** ***** ***** ***** ***** ***** *****