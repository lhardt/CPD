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

  Word(const Word& other){
    for(int i = 0; i < STR_SZ; ++i) text[i] = other.text[i];
    len = other.len;
    n_times = other.n_times;

  }
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

#define SWP(type, a,b) do{ type tmp; tmp = a; a = b; b = tmp; }while(0)

void remove_repeated(std::vector<Word> & list){
  for(int i = 1; i < list.size();++i){
    if( 0 == strcmp(list[i].text, list[i-1].text) ){
      list[i-1].n_times += list[i].n_times;
      list.erase(list.begin() + i);
      --i;
    }
  }
}


void radix_sort(std::vector<Word> & list, int last_pos){
  int count[256] = {0};

  std::vector<Word> list_copy(list);

  std::vector<Word> *list_a = &list;
  std::vector<Word> *list_b = &list_copy;

  int n_total = list.size();
  for(int pos = last_pos-1; pos >= 0; --pos){
    for(int i = 0; i < 256; ++i){ count[i] = 0; }
    SWP(std::vector<Word>*, list_a, list_b);
    // counts them all
    for(int i = 0; i < list_a->size(); ++i){
      Word this_word = list_a->at(i);
      if(this_word.len <= pos){
        count[0]++;
      } else {
        count[ this_word.text[pos] ]++;
      }
    }

    if(count[0] == list.size())
      break;

    // aggregates
    for(int i = 1; i < 256; ++i){
      count[i] += count[i-1];
    }
    // puts them in order
    for(int i = list.size()-1; i >= 0; --i){
      Word this_word = list_a->at(i);
      int new_pos;
      if(this_word.len <= pos){
        new_pos = --count[0];
        // printf("new pos is %d\n", new_pos);
      } else {
        new_pos = --count[ this_word.text[pos] ];
      }

      (*list_b)[new_pos] = list_a->at(i);

      // check if there are repeated words.
    }
  }
  // list_b was the copied_into from the last iteration
  for(int i = 0; i < list.size(); ++i){
    list[i] = list_b->at(i);
  }

  remove_repeated(list);
}

int main(){
  int last_pos = 0;
  std::vector<Word> list_of_words = read_file(last_pos);


  radix_sort(list_of_words, last_pos);

  for(auto word : list_of_words){
    std::cout << word;
  }
  printf("\nn_words: %d\n", list_of_words.size());

  return 0;
}
