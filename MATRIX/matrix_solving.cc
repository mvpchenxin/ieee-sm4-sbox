// matrix_sol.cc
// version August 5, 2024


/* On Aug.05, 2024, this was run on the matrix describing the
bottom and top linear transformations in our new SM4 S-box circuit
(maple program on file) . 


The matrices are r (bottom) =


0 1 1 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 
0 1 1 1 0 1 1 1 0 1 0 1 0 1 1 1 1 0
0 0 0 1 1 0 1 1 0 1 1 0 1 1 0 0 0 0
1 1 0 1 1 0 0 0 0 0 0 0 1 1 0 1 1 0
0 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 1 1 0 1 1 0 0 1 1 0 0 0 0 1 1 
0 1 1 1 0 1 1 1 0 0 0 0 1 1 0 1 1 0
1 1 0 1 1 0 0 0 0 0 1 1 1 1 0 1 0 1


The program output a circuit with 28 XOR/XNORs



and tlt (top) =

0 0 0 0 0 1 1 0
1 0 0 0 1 0 1 0
0 0 0 0 0 0 0 1
0 0 1 0 0 0 1 0
0 0 1 0 1 0 1 0
1 1 0 1 1 1 0 1
0 0 1 0 1 0 0 1
1 0 0 1 1 0 0 1
1 0 0 0 0 1 1 0
1 1 1 1 0 1 1 0
0 0 0 0 1 1 0 1
0 0 0 0 0 0 1 0
1 0 1 0 1 0 0 0
0 1 0 0 0 1 0 0
1 0 1 0 0 1 0 0 
0 1 0 0 0 1 1 0
0 0 1 0 0 0 1 1
1 0 1 0 0 0 0 0

The program output a circuit with 17 XORs

*/



#include <math.h>
#include <ctype.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

using namespace std;

const int MaxBaseSize=1000;
const bool PRINTROWS=true;

int NumInputs; 
int NumTargets;
int ProgramSize;
long long int Target[MaxBaseSize];
int Dist[MaxBaseSize]; 
int NDist[MaxBaseSize]; 
long long int Base[MaxBaseSize];
int BaseSize;
int TargetsFound;

void InitBase();
void ReadTargetMatrix();
bool is_target(long long int x);
bool is_base(long long int x);
int NewDistance(int u); 
int TotalDistance(); 
bool reachable(long long int T, int K, int S);
bool EasyMove(); 
void PickNewBaseElement();
void binprint(long long int x); 

ifstream TheMatrix;
//ofstream out_file;

int
main(int argc, char *argv[])
{
  int NumMatrices; 

  clock_t t1=clock();
  TheMatrix.open("tlt");
  //TheMatrix.open("r");
 
  TheMatrix >> NumMatrices;
  for (int i = 0; i < NumMatrices; i++)
  {
    ReadTargetMatrix(); 
    InitBase(); 
    ProgramSize = 0; 
    int counter = 0; 
    while (TargetsFound < NumTargets) 
    {
      counter++;
      //cout << counter << endl;
      if (!EasyMove()) PickNewBaseElement();
      //for (int dd = 0 ; dd < NumTargets; dd++) cout << Dist[dd] << " " ;
      //cout << endl;
    }
    cout << ProgramSize << endl;
  }
  clock_t t2=clock();
  cout << (t2-t1)/(double)CLOCKS_PER_SEC;
}

void InitBase()
{
  TargetsFound = 0;
  Base[0] = 1;
  for (int i = 1; i < NumInputs; i++) Base[i] = 2*Base[i-1];
  BaseSize = NumInputs; //initial base is just the xi's
  for (int i = 0; i < NumTargets; i++) 
    if (Dist[i] == 0) TargetsFound++;
}

int TotalDistance() 
{
  int D = 0;
  int t;
  for (int i = 0; i < NumTargets; i++) 
  {
    t = NewDistance(i); 
    NDist[i] = t; 
    D = D + t;
  }
  return D;
}

long long int NewBase; 

bool EasyMove()
{
  int t;
  bool foundone = false;
  
  
  for(int i = 0; i < NumTargets; i++) 
    if (Dist[i] == 1) 
    {
      foundone = true;
      t = i;
      break;
    }
  if (!foundone) return false;
  
  NewBase = Target[t]; 
  for (int u = 0; u < NumTargets; u++) Dist[u] = NewDistance(u); 
  //update Base with NewBase 更新基
  Base[BaseSize] = NewBase; 
  BaseSize++; 
  ProgramSize++; //尺寸自加
  TargetsFound++;
  return true;
} 


void PickNewBaseElement()
{
  int MinDistance;
  long long int TheBest;
  int ThisDist;
  int ThisNorm, OldNorm;
  int besti,bestj, d;
  bool easytarget;
  int BestDist[MaxBaseSize];

  MinDistance = BaseSize*NumTargets; //i.e. something big
  OldNorm = 0; 

  for (int i = 0; i < BaseSize - 1; i++)
  {
  for (int j = i+1; j < BaseSize; j++)
  {
    NewBase = Base[i] ^ Base[j];
  
    if (NewBase == 0) { cout << "a base is 0, should't happen " << endl; exit(0); }
    
    if (is_base(NewBase)) continue;
    
    easytarget = false;
    if (is_target(NewBase))
    {
      cout << "shouldn't find an easy target here " << endl; 
      exit(0);
      easytarget = true;
      besti = i;
      bestj = j;
      TheBest = NewBase;
      break;
    }
    ThisDist = TotalDistance(); 
    //cout << "i " << i << " j " << j << "  ThisDist " << ThisDist << endl;
    if (ThisDist <= MinDistance)
    {
      
      ThisNorm = 0; 
      for (int k = 0; k < NumTargets; k++)
      {
        d = NDist[k];
	ThisNorm = ThisNorm + d*d;
      }
     
      if ((ThisDist < MinDistance) || (ThisNorm > OldNorm) )
      {
        besti = i;
        bestj = j;
        TheBest = NewBase;
        for (int uu = 0; uu < NumTargets; uu++) BestDist[uu] = NDist[uu]; 
	MinDistance = ThisDist;
	OldNorm = ThisNorm;
      }
    }
  }
    if (easytarget) break;
  }
  
  NewBase = TheBest; 
  for (int i = 0; i < NumTargets; i++) Dist[i] = BestDist[i];
  
  Base[BaseSize] = TheBest;
  BaseSize++;
  
  ProgramSize++;
  //cout << ProgramSize << " : " ;
  //cout << "X" << BaseSize-1 << " = X" << besti << " + X" << bestj;
  //if (is_target(TheBest)) { cout << " * " ; if (PRINTROWS) binprint(TheBest);}
  //cout << endl;
  // if a target is found update counter
  if (is_target(TheBest)) TargetsFound++; 
} 

void binprint(long long int x) 
{
  long long int t = x;
  for (int i = 0; i < NumInputs; i++)
  {
    if (t%2) cout << "1 "; else cout << "0 ";
    t = t/2;
  }
} 

void ReadTargetMatrix()
{
  TheMatrix >> NumTargets;
  TheMatrix >> NumInputs;
  //check that NumInputs is < wordsize
  if (NumInputs >= 8*sizeof(long long int)) 
  {
    cout << "too many inputs" << endl;
    exit(0);
  }

  //cout << NumTargets << " equations and ";
  //cout << NumInputs << " variables" << endl;
  int bit;
  for (int i = 0; i < NumTargets; i++)
  //read row i
  {
    long long int PowerOfTwo  = 1;
    Target[i] = 0;
    Dist[i] = -1; 
    for (int j = 0; j < NumInputs; j++) 
    {
      TheMatrix >> bit;
      if (bit) 
      {
        Dist[i]++; 
	Target[i] = Target[i] + PowerOfTwo;
      }
      PowerOfTwo = PowerOfTwo * 2;
    }
  }
} 


bool is_target(long long int x)
{
  for (int i = 0; i < NumTargets; i++)
    if (x == Target[i]) return true;
  return false;
} //is_target

bool is_base(long long int x)
{
  //sanity check, shouldn't ask if 0 is base
  if (x==0) { cout << "asking if 0 is in Base " <<endl ; exit(0); }
  
  for (int i = 0; i < BaseSize; i++) if (x == Base[i]) return true;
  return false;
} 


  
int NewDistance(int u) 
{
  
  if (is_base(Target[u]) || (NewBase == Target[u])) return 0;
  

  
  if (reachable(Target[u] ^ NewBase,Dist[u]-1,0)) return (Dist[u]-1);
  else return Dist[u]; //keep old distance 
} 



bool reachable(long long int T, int K, int S)
{
    if ((BaseSize-S) < K) return false; 
    
    if (K==0) return false; 
    if (K==1) 
    {
      for (int i=S; i < BaseSize; i++) if (T == Base[i]) return true;
      return false;
    } 
    
    
    if (reachable(T^Base[S], K-1, S+1)) return true;
    
    if (reachable(T, K, S+1)) return true;
    //not found
    return false;
} 
