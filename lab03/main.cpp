#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>

#define STR_SZ 32

class Word {
public:
  Word()  { }
  ~Word() { }
  char text[STR_SZ];
  int  n_times;
  int  len;

  friend std::ostream &operator<<(std::ostream& os, const Word & word) {
    os << word.text << " " << word.n_times << std::endl;
    return os;
  }
  // friend
};

bool should_ignore(Word & word){
  int len = strlen(word.text);
  if(len < 4) return true;

  for(int i = 0; i < STR_SZ; ++i){
    if(isdigit(word.text[i])) return true;
  }

  return false;
}

void fix_word(Word & word){
  word.len = strlen(word.text);
  for(int i  = 0; i < word.len; ++i){
    word.text[i] = toupper(word.text[i]);
  }
}

std::vector<Word> read_file(int & max_len){
  std::vector<Word> vec;

  Word next_word;
  // Reads the words and clears out any trash
  while(EOF != scanf(" %[a-zA-z]%*[!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~0123456789]", next_word.text)){
    next_word.n_times = 1;
    fix_word(next_word);

    max_len = max_len > next_word.len ? max_len : next_word.len;

    if(should_ignore(next_word))
      continue;

    vec.push_back( next_word );
  }

  return vec;
}

void radix_sort(std::vector<Word> & list, int last_pos){
  int count[256] = {0};

  for(int i = 0; i < 256; ++i){ count[i] = 0; }

  int n_total = list.size();
  while(n_total != count[0]){

    for(int pos)
  }
}

int main(){
  int last_pos;
  std::vector<Word> list_of_words = read_file(&last_pos);

  for(auto word : list_of_words){
    std::cout << word;
  }

  radix_sort(list, last_pos);

  printf("n_words: %d\n", list_of_words.size());

  return 0;
}
