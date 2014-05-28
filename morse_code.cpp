#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char  alphabet2morse_code_raw[] = "\
A .-   B -... C -.-. D -.. \
E .    F ..-. G --.  H .... \
I ..   J .--- K -.-  L .-.. \
M --   N -.   O ---  P .--. \
Q --.- R .-.  S ...  T - \
U ..-  V ...- W .--  X -..-\
Y -.-- Z --..\
";
char alphabet2morse_code[26][10];
void ParseMorseCode() {
    char *s = alphabet2morse_code_raw;
    //printf("%s\n", s);
    int k = 0;
    for (int i = 0; s[i]; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            int j = 0;
            do {
                i++;
                if (!s[i] || (s[i] >= 'A' && s[i] <= 'Z'))
                    break;
                if (s[i] == '.' || s[i] == '-')
                    alphabet2morse_code[k][j++] = s[i];
            } while (1);
            alphabet2morse_code[k][j] = '\0';
            k++,i--;
        }
    }
    //for (int i = 0; i < 26; ++i) {
        //printf("%c\"%s\"\n", 'A' + i, alphabet2morse_code[i]);
    //}
}

struct Node {
    Node *next[2];
    int count;
};
Node container[1000000];
int container_idx = 0;
void ConstructTrie(Node *root, char *word) {
    Node *cur = root;
    for (int i = 0; word[i]; ++i) {
        char *s = alphabet2morse_code[word[i] - 'A'];
        for (int j = 0; s[j]; ++j) {
            int k = s[j] == '.' ? 0:1;
            if (cur->next[k] == NULL) {
                cur->next[k] = &container[container_idx++];
                cur->next[k]->count = 0;
                cur->next[k]->next[0] = NULL;
                cur->next[k]->next[1] = NULL;
            }
            cur = cur->next[k];
        }
    }
    cur->count++;
}


int Dynamic(Node *root, char *morse_code) {
    int dp[10000+10];
    memset(dp, 0, sizeof(dp));
    int len = strlen(morse_code);
    for (int i = 0; i < len; ++i) {
        //TraverseTrie
        Node *cur = root;
        int j = i;
        int pre_count = i == 0 ? 1:dp[i - 1];
        do {
            int k = morse_code[j] == '.' ? 0:1;
            if (cur->next[k] == NULL)
                break;
            cur = cur->next[k];
            dp[j] += pre_count*cur->count;
            j++;
        } while(j < len);
    }
    //for (int i = 0; i < len; ++i)
        //printf("%d ",dp[i]);
    //printf("\n");
    return dp[len - 1];
}

int main() {
    ParseMorseCode();
//1
//.---.--.-.-.-.---...-.---.
//6
//AT
//TACK
//TICK
//ATTACK
//DAWN
//DUSK
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    char morse_code[10000 + 10];
    int num_testcase;
    scanf("%d", &num_testcase);
    for (int t = 0; t < num_testcase; ++t) {
        container_idx = 0;
        scanf("%s", morse_code);
        int dict_num;
        scanf("%d", &dict_num);
        char word[30];
        Node *root = &container[container_idx++];
        root->next[0] = root->next[1] = NULL;
        root->count = 0;
        for (int i = 0; i < dict_num; ++i) {
            scanf("%s", word);
            ConstructTrie(root, word);
        }
        int ans = Dynamic(root, morse_code);
        printf("%d\n", ans);
    }
    return 0;
}
