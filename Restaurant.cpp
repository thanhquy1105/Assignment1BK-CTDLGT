#include "main.h"

class imp_res : public Restaurant
{
	private:
		customer* rtable = nullptr, *queue =nullptr, *order = nullptr;
		customer* posOfChange = nullptr;
		int tableSize = 0, queueSize = 0;

	public:	
		imp_res() {};
        ~imp_res() {
            customer* current;
            while (queue != nullptr){
                PopQueue();
            }

            // check
            while (order !=nullptr) {
                current = order;
                order = order->next;
                current->next = nullptr;
                current->prev = nullptr;
                delete current;
            }

            while (rtable != nullptr) {
                deleteCustomerAtTable(rtable);

            }
            
        };

        void PopQueue() {
            customer *current = queue;
            queue = queue->next;
            current->next = nullptr;
            current->prev = nullptr;
            delete current;
            queueSize-=1;
        }

        void deleteCustomerAtTable(customer* cus) {
            if (cus == nullptr) return;
            if (cus->next == cus && cus->prev == cus) {
                    customer*current = cus;
                    rtable = nullptr;
                    posOfChange =nullptr;
                    current->next =nullptr;
                    current->prev = nullptr;
                    delete current;
            } else {
                customer*prev = cus->prev;
                prev->next = cus->next;
                cus->next->prev = prev;
                if (cus == rtable) {
                    rtable = cus->next;
                }
                posOfChange = cus->next;
                cus->next = nullptr;
                cus->prev = nullptr;
                delete cus;
            }
            tableSize -=1;
            return;
        }

		void RED(string name, int energy)
		{
			if (energy == 0) return;
			cout <<"RED: " << name << " " << energy << endl;
            // nil table
			if (rtable == nullptr && tableSize < MAXSIZE) {
			    customer *cus = new customer (name, energy, nullptr, nullptr);
                customer *cusOrder = new customer(name, energy, cus, nullptr);
                cus->next = cus;
                cus->prev = cus;
				rtable = cus;
                tableSize++;
                order = cusOrder;
                posOfChange = cus;
				return;
			} 
            // out of size
            if (tableSize >= MAXSIZE && queueSize >= MAXSIZE) return;
            // invalid name: for rtable.name == name || queue.name == name ? return
            customer* current = rtable;
            do {
                if (current->name == name) return;
                current = current->next;
            } while (current != rtable);
            current = queue;
            while (current != nullptr) {
                if (current->name == name) return;
                current = current->next;
            }
            customer *cus = new customer (name, energy, nullptr, nullptr);
            if (tableSize < MAXSIZE/2) {
                if (cus->energy >= posOfChange->energy) {
                    customer*temp = posOfChange->next;
                    posOfChange->next=cus;
                    cus->prev = posOfChange;
                    cus->next = temp;
                    temp->prev = cus;
                } else {
                    customer*temp = posOfChange->prev;
                    posOfChange->prev=cus;
                    cus->next = posOfChange;
                    cus->prev = temp;
                    temp->next = cus;
                }
                tableSize++;
                posOfChange= cus;

                // Add to order
                customer *cusOrder = new customer(name, energy, cus, nullptr);
                customer *current = order;
                while (current->next !=nullptr) {
                    current = current->next;
                }
                current->next = cusOrder;

            }else if (tableSize < MAXSIZE) {
                customer * biggest = rtable;
                int max = abs(cus->energy - biggest->energy);
                customer *current = rtable->next;
                while (current != rtable) {
                    int temp = abs(cus->energy - current->energy);
                    if (temp > max) {
                        biggest = current;
                        max = temp;
                    }
                    current = current->next;
                }
            
                int res = cus->energy - biggest->energy;
                if (res < 0) {
                    customer*temp = biggest->prev;
                    biggest->prev = cus;
                    cus->next = biggest;
                    cus->prev = temp;
                    temp->next = cus;
                } else {
                    customer*temp = biggest->next;
                    biggest->next=cus;
                    cus->prev = biggest;
                    cus->next = temp;
                    temp->prev = cus;
                }
                tableSize++;
                posOfChange= cus;
                // Add to order
                customer *cusOrder = new customer(name, energy, cus, nullptr);
                current = order;
                while (current->next !=nullptr) {
                    current = current->next;
                }
                current->next = cusOrder;

            }else {
                // push into queue
                if (queue == nullptr) {
                    queue = cus;
                } else {
                    customer* current = queue;
                    while (current->next !=nullptr) {
                        current = current->next;
                    };
                    current->next = cus;
                    cus->prev = current;
                }
                queueSize++;
            }
            
		}
		
        void BLUE(int num)
		{
			cout << "BLUE " << num << endl;
			int i=0;
            while (i<num &&order !=nullptr) {
                customer *customerAtTable = order->prev;
                customer*temp = order;
                order = order->next;
                i++;
                temp->next = temp->prev = nullptr;
                delete temp;
                deleteCustomerAtTable(customerAtTable);
            }
            while (tableSize < MAXSIZE && queue != nullptr) {
                string name = queue->name;
                int energy = queue->energy;
                PopQueue();
                RED(name, energy);
            }
            
		}

        void swapQueue(customer* cus1, customer *cus2) { // vi tri i < vi tri cus2
            if (cus1 == cus2 ) return;
            if (cus1 == queue) queue = cus2;
            if (cus1->next == cus2) {
                customer *prevcus1 = cus1->prev,*nextcus2 = cus2->next;
                cus2->next =cus1;
                cus2->prev = prevcus1;
                if (prevcus1 != nullptr) prevcus1->next = cus2;
                cus1->next = nextcus2;
                cus1->prev = cus2;
                if (nextcus2!=nullptr) nextcus2->prev = cus1;
            }else {
                customer *prevcus1 = cus1->prev,*nextcus1=cus1->next, *prevcus2 = cus2->prev,*nextcus2 = cus2->next;
                if (prevcus1 != nullptr) prevcus1->next =cus2;
                cus2->next = nextcus1;
                cus2->prev = prevcus1;
                nextcus1->prev = cus2;

                prevcus2->next = cus1;
                cus1->prev = prevcus2;
                cus1->next = nextcus2;
                if (nextcus2!=nullptr) nextcus2->prev = cus1;
            }
        }

        void insort(customer**from, customer** to, int increase,int *timeSwap) {
            for (customer* i=*from; ;)  {
                bool check = false;
                for (customer* j=i; ; ) {
                    cout<<endl;
                    (*from)->print();
                    LIGHT(0);
                    // y = j - incr
                    int y=0;
                    customer *compare = j;
                    do {
                        y++;
                        compare = compare->prev;
                        if (compare==nullptr) break;
                        if (compare == (*from)->prev) check = true;
                    } while (y!=increase);

                    if (compare==nullptr) return;

                    bool equalSwap = false;
                    if (abs(j->energy) == abs(compare->energy)) {
                        if (j->name[j->name.length()-1] < compare->name[compare->name.length()-1]) {
                            equalSwap = true;
                        }
                    }

                    // if (j >= incr) && (arr[j] > arr[j-incr]) {
                    if (equalSwap||abs(j->energy) > abs(compare->energy)) {
                        if (compare == *from) {
                            *from = j;
                        } else if (j == *from) {
                            *from =compare;
                        }
                        if (j == *to) {
                            *to = compare;
                        }
                        if (i == j) i = compare;
                        *timeSwap = *timeSwap + 1;
                        swapQueue(compare, j);
                        if (check== true) break;
                        
                    } else break;
                }
                // ;i!=to;i+=incr
                int x=0;
                do {
                    x++;
                    i = i->next;
                    if (i == (*to)->next) return;
                } while (x!=increase);
                
            }
        }

		void PURPLE()
		{
			cout << "PURPLE"<< endl;
            if (queue == nullptr) return ;
            int max = 0,x=1,posMax = 1;
            customer*cur = queue,*maxAbs = queue;
            while (cur != nullptr) {
                x++;
                if (abs(cur->energy) >= max) {
                    max = abs(cur->energy);
                    maxAbs = cur;
                    posMax = x;
                }
                cur->name = cur->name + to_string(x);
                cur = cur->next;
            }
            cout << "POS OF ABS MAX: " << posMax << endl; 
            cout << "ABS MAS: (" << maxAbs->name << "," << maxAbs->energy << ")" << endl; 
            if (maxAbs == queue) {
                cur = queue;
                while (cur !=nullptr)
                {
                    cur->name.pop_back();
                    cur = cur->next;
                }
                
                return;
            }
            int timeSwap = 0;
            // ToDO: shell sort from queue to maxAbs
            for(int i=posMax/2;i>2;i/=2) {
                customer*from = queue;
                int y =0;
                while (y<i) {
                    y++;
                    from = from->next;
                }
                int j=0;
                for (;j<i;) {
                    cout <<endl<< "outside from: ";
                    from->print();
                    insort(&from,&maxAbs,i,&timeSwap);
                    j++;
                    from = from->next;
                    if (from == nullptr) break;
                }
            }
            customer *from = queue->next;
            insort(&from,&maxAbs,1,&timeSwap);

            cout << "SWAP TIMES: "<<timeSwap<<endl;
            
            cur = queue;
            while (cur !=nullptr)
            {
                cur->name.pop_back();
                cur = cur->next;
            }
            
            BLUE(timeSwap%MAXSIZE);
		}
		
         void swapRTable(customer* i, customer *j) { // vi tri i < vi tri j
            if (i == j ) return;
            if (i == rtable) rtable = j;
            if (i->next == j) {
                customer *prevI = i->prev,*nextJ = j->next;
                j->next =i;
                j->prev = prevI;
                if (prevI == posOfChange) {
                    prevI->prev = j;
                } else {
                    prevI->next = j;
                }

                i->next = nextJ;
                i->prev = j;
                nextJ->prev = i;
            }else {
                customer *prevI = i->prev,*nextI=i->next, *prevJ = j->prev,*nextJ = j->next;
                j->next = nextI;
                j->prev = prevI;
                if (prevI == posOfChange) {
                    prevI->prev = j;
                } else {
                    prevI->next = j;
                }
                nextI->prev = j;

                prevJ->next = i;
                i->prev = prevJ;
                i->next = nextJ;
                nextJ->prev = i;
            }
        }

        void REVERSAL()
		{
			cout << "REVERSAL" << endl;
            if (posOfChange == nullptr) return;
            
            cout << "XPOS: (" << posOfChange->name << "," << posOfChange->energy << ")" <<endl; 
            customer*currentLeft = posOfChange, *prev = posOfChange->next, 
                    *next = nullptr, *currentRight = posOfChange->next;
            bool swap = false, across = false;
            do {
                next = currentLeft->prev;
                if (currentRight == next) across = true;
                if (next->energy * posOfChange->energy > 0 || swap || across) {
                    currentLeft->next = next;
                    currentLeft->prev = prev;
                    prev = currentLeft;
                    currentLeft = next;
                    swap = false;
                }else {
                    while (currentRight->energy *posOfChange->energy > 0) {
                        currentRight = currentRight->next;
                        if (currentRight == next) {
                            across = true;
                            break;
                        }
                    }
                    if (currentRight != next) {
                        if (currentRight->next == next && next->energy*currentRight->energy > 0) {
                            across = true;
                        }
                        swapRTable(currentRight,next);
                        currentRight = next->next;
                        swap=true;
                    }
                    
                }
            }while(!(currentLeft == posOfChange && across == true));

            posOfChange->prev = prev;
            // print
            customer*curre = rtable;
            cout << "Round Table After Reverse:" << tableSize <<endl;
            if (rtable != nullptr) {
                do {
                    cout << "(" << curre->name << "," << curre->energy << ")" << "->"; 
                    curre = curre->next;
                }while(curre != rtable);
                cout <<endl;
                curre = rtable;
                do {
                    cout << "(" << curre->name << "," << curre->energy << ")" << "->"; 
                    curre = curre->prev;
                }while(curre != rtable);
                cout <<endl;
            }
		}
		
        void UNLIMITED_VOID()
		{
			cout << "UNLIMITED_VOID" << endl;
            if (tableSize < 4) return;
            cout <<"XPOS: (" << posOfChange->name << "," << posOfChange->energy << ")" << endl;
            customer*current = posOfChange;
            struct {
                int sum =INT_MAX;
                customer*begin = nullptr;
                customer *end = nullptr;
                customer *min = nullptr;
            } res; 
            
            do {
                customer*temp = current;
                customer*min= current;
                int i=1,sum = 0;
                do {
                    if (min->energy > temp->energy) min = temp;
                    sum +=temp->energy;
                    if (i>=4 && sum <= res.sum) {
                        res.sum = sum;
                        res.begin = current;
                        res.end = temp;
                        res.min = min;
                    }
                    i++;
                    temp = temp->next;
                }while (temp != current);
                current = current->next;
            }while (current != posOfChange);
            cout << "SUM: " << res.sum << endl;
            cout <<"MIN (" << res.min->name << "," << res.min->energy << ")" << endl;
            cout <<"BEGIN: (" << res.begin->name << "," << res.begin->energy << ")" << "->";
            cout <<"END: (" << res.end->name << "," << res.end->energy << ")" << endl<<endl;

            if (res.begin ==nullptr) return;
            current = res.min;
            do {    
                cout <<"(" << current->name << "," << current->energy << ")" << "->";
                if (current == res.end) {
                    current = res.begin;
                }else current = current->next;
            } while (current != res.min);
            cout <<endl<<endl;
		}

        void addToStack(customer **stack, customer*cus) {
            if (*stack == nullptr) *stack = cus;
            else {
                cus->next = *stack;
                *stack = cus; 
            }
        }
		
        void DOMAIN_EXPANSION()
		{
            cout << "DOMAIN_EXPANSION" << endl;
			int sumPositive = 0, sumNegative = 0;
            customer* currentRT = rtable;
            if (currentRT == nullptr) { return;	}
            do {
                if (currentRT->energy > 0) {
                    sumPositive += currentRT->energy; 
                }
                else {
                    sumNegative += currentRT->energy; 
                }
                currentRT = currentRT->next; 
            } while (currentRT != rtable); 

            customer* currentQueue = queue;
            while (currentQueue != nullptr) {
                if (currentQueue->energy > 0) {
                    sumPositive += currentQueue->energy;
                }
                else {
                    sumNegative += currentQueue->energy; 
                }
                currentQueue = currentQueue->next; 
            }

            cout << "Positive Sum: " << sumPositive << ". Negative Sum: "<< sumNegative<<endl;
            customer* stack1 = nullptr; 
            if (sumPositive >= abs(sumNegative)) {
                customer* currentOrder = order;
                customer* prevOrder = nullptr; 
                while (currentOrder != nullptr) {
                    if (currentOrder->energy < 0) {
                        customer* cusTable = currentOrder->prev; 
                        customer* currentDel = new customer(cusTable->name, cusTable->energy, nullptr, nullptr);
                        customer* temp = currentOrder; 

                        if (currentOrder == order) {
                            order = order->next; }
                        else {
                            prevOrder->next = currentOrder->next; 
                        }

                        currentOrder = currentOrder->next;	
                        temp->next = temp->prev = nullptr;
                        delete temp; 
                        deleteCustomerAtTable(cusTable);
                        addToStack(&stack1,currentDel);
                    }
                    else {
                        prevOrder = currentOrder;
                        currentOrder = currentOrder->next;
                    }
                }

                customer* currentQueue = queue;
                customer* prevQueue = nullptr;
                while (currentQueue != nullptr) {
                    if (currentQueue->energy <0) {
                        customer* currentDel = new customer(currentQueue->name, currentQueue->energy, nullptr, nullptr);
                        customer*temp = currentQueue;
                        if (temp == queue) {
                            queue = queue->next;
                        }else {
                            prevQueue->next = currentQueue->next;
                        }
                        currentQueue= currentQueue->next;
                        temp->next = temp->prev = nullptr;
                        delete temp;
                        addToStack(&stack1,currentDel);
                    }
                    else {
                        prevQueue = currentQueue;
                        currentQueue = currentQueue->next;
                    }
                } 
                
            } 
            else {
                customer* currentOrder = order;
                customer* prevOrder = nullptr; 
                while (currentOrder != nullptr) {
                    if (currentOrder->energy > 0) {
                        customer* cusTable = currentOrder->prev; 
                        customer* currentDel = new customer(cusTable->name, cusTable->energy, nullptr, nullptr);
                        customer* temp = currentOrder; 

                        if (currentOrder == order) {
                            order = order->next; }
                        else {
                            prevOrder->next = currentOrder->next; 
                        }

                        currentOrder = currentOrder->next;	
                        temp->next = temp->prev = nullptr;
                        delete temp; 
                        deleteCustomerAtTable(cusTable);
                        addToStack(&stack1,currentDel); 
                    }
                    else {
                        prevOrder = currentOrder;
                        currentOrder = currentOrder->next;
                    }
                }
                customer* currentQueue = queue;
                customer* prevQueue = nullptr;
                while (currentQueue != nullptr) {
                    if (currentQueue->energy > 0) {
                        customer* currentDel = new customer(currentQueue->name, currentQueue->energy, nullptr, nullptr);
                        customer*temp = currentQueue;
                        if (temp == queue) {
                            queue = queue->next;
                        }else {
                            prevQueue->next = currentQueue->next;
                        }
                        currentQueue= currentQueue->next;
                        temp->next = temp->prev = nullptr;
                        delete temp;
                        addToStack(&stack1,currentDel);
                    }
                    else {
                        prevQueue = currentQueue;
                        currentQueue = currentQueue->next;
                    }
                } 
            }

            // In
            while (stack1!=nullptr) {
                cout << "(" << stack1->name << "," << stack1->energy << ")" << "->"; 
                customer*temp = stack1;
                stack1 = stack1->next;
                temp->next = temp->prev = nullptr;
                delete temp;
            }
            cout <<endl;

            while (tableSize < MAXSIZE && queue != nullptr) {
                    string name = queue->name;
                    int energy = queue->energy;
                    PopQueue();
                    RED(name, energy);
            }
            
		}

		void LIGHT(int num)
		{
            cout << endl; 
            cout << "LIGHT " << num << endl; 
            if (posOfChange== nullptr) return; // CHECK
            customer* currentRT = posOfChange; 
            cout << "Current X Position: " << posOfChange->name << "-" << posOfChange->energy << endl; 
            if (num > 0 && currentRT != nullptr) { 
                do {
                    //currentRT->print(); 
                    cout << "(" << currentRT->name << "," << currentRT->energy << ")" << "->";
                    currentRT = currentRT->next;
                } while (currentRT != posOfChange); 
            }
            else if (num < 0 && currentRT != nullptr) {
                do {
                    //currentRT->print(); 
                    cout << "(" << currentRT->name << "," << currentRT->energy << ")" << "->";
                    currentRT = currentRT->prev;
                } while (currentRT != posOfChange); 
            }
            else if (num == 0){
                customer* currentQueue = queue;
                while (currentQueue != nullptr) {
                    //currentQueue->print(); 
                    cout << "(" << currentQueue->name << "," << currentQueue->energy << ")" << "->";
                    currentQueue = currentQueue->next; 
                }
            } 
		}
};


// MAXSIZE 10
// RED ABC -123
// RED BCD 156
// RED DS 56
// RED ABC -6
// RED AAA -21
// RED GHW 0
// RED FF -322
// RED DSS 111
// RED BCD 456
// RED FD 456
// RED BBS -234
// RED GGG -34
// RED RWE -342
// RED QWQ 934
// RED FSSD 23
// RED FGD -54
// RED FYD -174
// RED TSD 84

// PURPLE
// REVERSAL
// UNLIMITED_VOID
// DOMAIN_EXPANSION
// LIGHT 3

// MAXSIZE 8
// RED BCD 14
// RED DS -33
// RED ABC -3
// RED AAA 1
// RED GHW -45
// RED AC -20

// DOMAIN_EXPANSION

// MAXSIZE 7
// RED A 1
// RED B 3
// RED C 2
// RED D 4
// RED E 6
// RED F 9
// RED G 8

// RED ABC 1
// RED BCD 7
// RED DS -8
// RED GHW -2
// RED AAA 7
// RED ASD 15
// RED GFD -1

// PURPLE


// MAXSIZE 8
// RED A 1
// RED B 3
// RED C 2
// RED D 4
// RED E 6
// RED F 9
// RED G 8
// RED H 5

// RED ABC 1
// RED BCD 7
// RED DS -8
// RED GHW -2
// RED AAA 7
// RED GFD -1
// RED GIM 2
// RED ASD 15

// LIGHT 3
// LIGHT 0

// PURPLE