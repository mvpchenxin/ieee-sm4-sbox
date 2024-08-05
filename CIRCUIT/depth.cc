// Circuit_depth
// Aug 05, 24

const int MaxCircuitSize = 1000;
const int NOVALUE = -1 ;
const int INPUT = -1 ;
const char NO_OP = ' ';

int Max(int , int ) ;

class circuit_node{
                       public: 
		         int f; 
			 int operand1; 
			 int operand2; 
			 int val; 
			 int output_num; 
			 bool check_mark; 
			 bool output_gate; 
			 circuit_node(void); 
                         bool is_input(void); 
			                      
                			 
		       private:
		     };

circuit_node::circuit_node(void) 
{
  operand1 = INPUT;
  operand2 = INPUT;
  val = NOVALUE;
  check_mark = false;
  output_gate = false; 
  output_num = NOVALUE;
}

class circuit{
               public: 
	         int NUMGATES;
		 circuit_node Gate[MaxCircuitSize];
		 void display_circuit(); 
		 void output_gdl(); 
		 int bineval(int);  
		 circuit(void); 
		 void add(int g1, int g2); 
		 void clear_values();
		 void clear_checks();
                 bool is_ancestor(int i, int j); 
		 void clear_circuit(void); 
		 void init(int); 
		 
		 int node_depth(int); 
		 int circuit_depth(); 
	       private:
	         void traverse(int); 
                 void traverse_gdl(int); 
		 void DoOutputGate(int gate_num);
	      };

void circuit::clear_circuit(void)
{
  for(int i = 0; i < NUMGATES; i++)
  {
    Gate[i].operand1 = INPUT;
    Gate[i].operand2 = INPUT;
    Gate[i].val = NOVALUE;
    Gate[i].check_mark = false;
    Gate[i].output_gate = false;
  }
}

int circuit::node_depth(int NodeNumber)
{
  if (Gate[NodeNumber].is_input()) return 0;
  return (1 + Max(node_depth(Gate[NodeNumber].operand1), node_depth(Gate[NodeNumber].operand2) ) );
}

int circuit::circuit_depth()
{
  int d,t;
  d = 0;
  for (int i = 0; i<NUMGATES; i++)
  {
    t = node_depth(i); 
    if (t > d) d = t;
  }
  return d;
}

void circuit::init(int inputs)
{
  clear_circuit();
  for(int i = 0; i < inputs; i++)
  {
    Gate[i].operand1 = INPUT;
    Gate[i].operand2 = INPUT;
  }
  NUMGATES = inputs;
}

//void circuit::find_outputs()
//{
//  for (int i=0; i < NUMGATES; i++) //see if i is an output gate
//  {
//    Gate[i].output_gate = true;
//    for (int j=0; j < NUMGATES; j++) //see if i is an operand of j;
//      if ((Gate[j].operand1 == i) || (Gate[j].operand2 == i) ) 
//      {
//        Gate[i].output_gate = false;
//	break;
//     }
//  }
//}

void circuit::add(int g1, int g2)
{
  
  if ((g1 >= NUMGATES) || (g1 < 0)) {cout << "error in add(), g1 out of range" << endl; exit(0);}
  if ((g2 >= NUMGATES) || (g2 < 0)) {cout << "error in add(), g2 out of range" << endl; exit(0);}
  Gate[NUMGATES].operand1 = g1;
  Gate[NUMGATES].operand2 = g2;
  Gate[NUMGATES].f = Gate[g1].f ^ Gate[g2].f;
  NUMGATES++;
}

circuit::circuit(void) 
{ 
  NUMGATES = 0;
}

void circuit::display_circuit()
{
  for(int i = 0; i < NUMGATES; i++) traverse(i); 
  clear_checks();
}

void circuit::output_gdl()
{
  //find_outputs();
  cout << "graph:{" << endl;
  for(int i = 0; i < NUMGATES; i++) traverse_gdl(i); 
  clear_checks();
  cout << "}" << endl;
}

void circuit::clear_checks(void)
{
  for (int i = 0; i < NUMGATES; i++) Gate[i].check_mark = false;
}

void circuit::DoOutputGate(int gate_num)
{
    cout << "node: {title:\"y" << gate_num << "\" label:\"y"<<Gate[gate_num].output_num;
    cout <<"\" borderwidth:0}"  << endl;
    cout << "edge:{source:\""<< gate_num << "\" target:\"y" <<gate_num<<"\"}"<< endl;
} 

void circuit::traverse_gdl(int gate_num) //this traverses UP
{
  if (Gate[gate_num].check_mark) return;
  Gate[gate_num].check_mark = true;
  if (Gate[gate_num].is_input())
  {
    cout << "node: {title:\"" << gate_num << "\" label:\"x"<<gate_num;
    cout <<"\" borderwidth:0}"  << endl;
    if (Gate[gate_num].output_gate)  DoOutputGate(gate_num);
    return;
  }
  
  if (Gate[gate_num].output_gate)
  {
    //cout << "node: {title:\"" << gate_num << "\" label:\"y"<< Gate[gate_num].output_num;
    cout << "node: {title:\"" << gate_num << "\" label:\"+";
    cout << "\" shape:circle}"  << endl;
    DoOutputGate(gate_num);
  }
  else 
  {
    //cout << "node: {title:\"" << gate_num << "\" label:\""<<gate_num<<"\" shape:circle}"  << endl;
    cout << "node: {title:\"" << gate_num << "\" label:\"+";
    cout << "\" shape:circle}"  << endl;
  } 
  cout << "edge:{source:\""<< Gate[gate_num].operand1 << "\" target:\"" <<gate_num<<"\"}"<< endl;
  cout << "edge:{source:\""<< Gate[gate_num].operand2 << "\" target:\"" <<gate_num<<"\"}"<< endl;
  traverse_gdl(Gate[gate_num].operand1);
  traverse_gdl(Gate[gate_num].operand2);
}

void circuit::traverse(int gate_num) 
{
  if (Gate[gate_num].check_mark) return;
  Gate[gate_num].check_mark = true;
  if (Gate[gate_num].is_input())
  {
    cout << "input variable " << gate_num << endl;
    return;
  }
  cout << gate_num <<  " = "
           << Gate[gate_num].operand1 << " + " << "  " 
	   << Gate[gate_num].operand2 << endl;

  traverse(Gate[gate_num].operand1);
  traverse(Gate[gate_num].operand2);
}//traverse

int circuit::bineval(int gate_num)
{
  if (Gate[gate_num].val == NOVALUE) 
  if (Gate[gate_num].is_input())
  {
    cout << "error, no input value for gate " << gate_num << endl;
    exit(0);
  }
  else
  {
      int result = bineval(Gate[gate_num].operand1) ^ 
                   bineval(Gate[gate_num].operand2) ;
      Gate[gate_num].val = result;
      return result;
  }
  return(Gate[gate_num].val);
}

void circuit::clear_values(void)
{
  for (int i = 0; i < NUMGATES; i++) Gate[i].val = NOVALUE;
}

bool circuit_node::is_input(void)
{
  return ((operand1 == INPUT) && (operand2 == INPUT));
}

bool circuit::is_ancestor(int i, int j)
{
  if (Gate[j].is_input()) return false;
  if (Gate[j].operand1 == i) return true;
  if (Gate[j].operand2 == i) return true;
  if (is_ancestor(i,Gate[j].operand1)) return true;
  if (is_ancestor(i,Gate[j].operand2)) return true;
  return false;
}

int Max(int x, int y)
{
  if (x > y) return x;
  return y;
}
