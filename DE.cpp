// Knapsack Problem using Differential Evoluation

#include<bits/stdc++.h>
using namespace std;

#define mutationFactor 0.8
#define crossoverRate 0.5
#define maxWeightOfSingleItem 5
#define maxBenefitOfSingleItem 20

#define totalGenerations 39
#define knapsackTotalWeight 30

#define totalNumberOfItems 20
#define populationSize 5

int temp[totalNumberOfItems+10]; //used in initVector
int tempWeight;                  //used in initVector
int tempBenefit;                 //used in initVector
int totalFitness;
int benefit[totalNumberOfItems+10];
int weight[totalNumberOfItems+10];

int currentPopulation[populationSize+10][totalNumberOfItems+10];
int sumOfWeight[populationSize+10];
int currentBenifit[populationSize+10];

int trialCurrentPopulation[populationSize+10][totalNumberOfItems+10];
int trialSumOfWeight[populationSize+10];
int trialCurrentBenifit[populationSize+10];


void assignBenifitAndWeight(){
    for(int i=0; i<totalNumberOfItems; i++){
        benefit[i] = rand() % maxBenefitOfSingleItem + 1;     // benefit in the range 1 to 100
        weight[i] = rand() % maxWeightOfSingleItem + 1;     // weight in the range 1 to 10
    }
}

void printBenifitAndWeight(){
    cout<<"\nBenefit: ";
    for(int i=0; i<totalNumberOfItems; i++){
        cout<<benefit[i]<<" ";
    }  
    cout<<endl;
    cout<<"Weight: ";
    for(int i=0; i<totalNumberOfItems; i++){
        cout<<weight[i]<<" ";
    }
    cout<<endl;
}

void printInputForDp(){
    cout<<knapsackTotalWeight<<endl;
    cout<<totalNumberOfItems<<endl;
    for(int i=0; i<totalNumberOfItems; i++){
        cout<<benefit[i]<<" ";
    }  
    cout<<endl;
    for(int i=0; i<totalNumberOfItems; i++){
        cout<<weight[i]<<" ";
    }
    cout<<endl;
}

void initVector(){
    while(true){
        
        tempWeight = 0;
        tempBenefit = 0;
        
        for(int i=0; i<totalNumberOfItems; i++){
            temp[i] =  rand() % 2; // current dimesion either 0 or 1
            // cout<<"temp[i] = "<< temp[i]<<" ";
            tempWeight += (temp[i] * weight[i]);
            tempBenefit += (temp[i] * benefit[i]);

        }

        if(tempWeight<=knapsackTotalWeight){
            break;
        }

    }
}

void initPopulation(){
    for(int i=0; i<populationSize; i++){
        initVector();
        for(int j=0; j<totalNumberOfItems; j++){
            currentPopulation[i][j] = temp[j];
            
        }

        sumOfWeight[i] = tempWeight;
        currentBenifit[i] = tempBenefit;
    }
}

void printCurrentPopulation(){
    
    for(int i=0; i<populationSize; i++){
        for(int j=0; j<totalNumberOfItems; j++){
            cout<<currentPopulation[i][j]<<" ";
        }
        cout<<", SumOfWeight = "<<sumOfWeight[i];
        cout<<", CurrentBenifit = "<<currentBenifit[i];
        
        cout<<endl;
    }
}

void printTrailCurrentPopulation(){
    for(int i=0; i<populationSize; i++){
        int totalBenefit=0;
        int totalWeight=0;
        for(int j=0; j<totalNumberOfItems; j++){
            cout<<trialCurrentPopulation[i][j]<<" ";
            // totalBenefit += trialCurrentPopulation[i][j]*benefit[j];

        }
        cout<<endl;
    }
}

bool checkConstraints(int member){
    
    int sum=0;
    for(int j=0; j<totalNumberOfItems; j++){
        sum += (weight[j]*trialCurrentPopulation[member][j]);
    }    
    if(sum<=knapsackTotalWeight){
        return true;
    }
    else{
        return false;
    }
}

void repair(int member){
    initVector();
    for(int j=0; j<totalNumberOfItems; j++){
        trialCurrentPopulation[member][j] = temp[j];
    }

}

void mutation(){
    //DE/rand/1 scheme

    int differenceVector1;
    int differenceVector2;
    int baseVector;
    int mutantVector;
    // 1 / (1 + exp(-net));  sigmoid function 0 to 1 range
    for(int i=0; i<populationSize; i++){
        mutantVector = i;
        while(1){
            baseVector = rand()%populationSize;
            if(baseVector != mutantVector){
                break;
            }
        }
        while(1){
            differenceVector1 = rand()%populationSize;
            if(differenceVector1 != baseVector && differenceVector1 != mutantVector){
                break;
            }
        }
        while(1){
            differenceVector2 = rand()%populationSize;
            if(differenceVector2 !=baseVector  && differenceVector2 != differenceVector1 && differenceVector2!= mutantVector){
                break;
            }
        }

        for(int j=0; j<totalNumberOfItems; j++){
            float mutantBit = currentPopulation[baseVector][j] + (mutationFactor) * ( currentPopulation[differenceVector1][j] - currentPopulation[differenceVector2][j] )  ;

            if(mutantBit == 1.0){
                trialCurrentPopulation[mutantVector][j] = 1;
            }
            else if(mutantBit == 0.0){
                trialCurrentPopulation[mutantVector][j] = 0;
            }
            else{
                //use of sigmoidal function
                float x = 1 / (1 + exp(-mutantBit));
                float randomNumber = (float)(rand()%10)/10; //between 0 and 1 in decimals
                if(randomNumber <= x){
                    trialCurrentPopulation[mutantVector][j] = 1;
                }
                else{
                    trialCurrentPopulation[mutantVector][j] = 0;
                }
            }
        }

        if(!checkConstraints(mutantVector)){
            repair(mutantVector);
        }
    }

}   

void crossover(){
    for(int i=0; i<populationSize; i++){
        int trialVector = i;
        for(int j=0; j<totalNumberOfItems; j++){
            float randomNumber = (float)(rand()%10)/10; //between 0 and 1 in decimals
            if( randomNumber > crossoverRate ){
                trialCurrentPopulation[trialVector][j] = currentPopulation[trialVector][j];
            }        
        }
        if(!checkConstraints(trialVector)){
            repair(trialVector);
        }
    }
}

void selection(){
     
    //calculation of trial population fitness
    for(int i=0; i<populationSize; i++){
        tempBenefit = 0;
        tempWeight = 0;        
        
        for(int j=0; j<totalNumberOfItems; j++){
            tempBenefit += (trialCurrentPopulation[i][j] * benefit[j]);
            tempWeight += (trialCurrentPopulation[i][j] * weight[j]);
        }
        trialCurrentBenifit[i] = tempBenefit;
        trialSumOfWeight[i] = tempWeight;
    }

    //selection based on fitness
    for(int i=0; i<populationSize; i++){
        if(trialCurrentBenifit[i] > currentBenifit[i]){
            for(int j=0; j<totalNumberOfItems; j++){
                currentPopulation[i][j] = trialCurrentPopulation[i][j];
            }
            sumOfWeight[i] = trialSumOfWeight[i];
            currentBenifit[i] = trialCurrentBenifit[i];
        }
    }   
}

void calculateTotalFitness(){
    totalFitness = 0;
    for(int i=0; i<populationSize; i++){
        totalFitness += currentBenifit[i];
    }
    cout<<"=======================================>Total Fitness = "<<totalFitness<<"\n";
}
int main(){
    FILE *fin1 = freopen("ouputByDE.txt", "w", stdout);
    cout<<"Knapsack Problem using Differential Evoluation\n\n";
    
    cout<<"mutationFactor = "<<mutationFactor;
    cout<<", crossoverRate = "<<crossoverRate;
    cout<<endl;  
    cout<<"maxWeightOfSingleItem = "<<maxWeightOfSingleItem;
    cout<<", maxBenefitOfSingleItem = "<<maxBenefitOfSingleItem;
    cout<<endl;
    cout<<"totalGenerations = "<<totalGenerations;
    cout<<", knapsackTotalWeight = "<<knapsackTotalWeight;
    cout<<endl;
    cout<<"totalNumberOfItems = "<<totalNumberOfItems;
    cout<<", populationSize = "<<populationSize;
    cout<<endl;

    int i,j,k,t,n;
    

    assignBenifitAndWeight();
    
    FILE *fin2 = freopen("inputForDP.txt", "w", stdout);
    printInputForDp();

    fin1 = freopen("ouputByDE.txt", "a+", stdout);
    
    printBenifitAndWeight();
    initPopulation();
    cout<<"**************************************************************\n";
    cout<<"\nCurrent Population: \n";
    printCurrentPopulation();
    calculateTotalFitness();    

    int currentGeneration=0;
    
    //iterations for generations
    while(currentGeneration< totalGenerations){  
        cout<<"**************************************************************\n";
        cout<<"Current Generation="<<currentGeneration<<"\n";    
        
        mutation();
        cout<<"\nAfter Mutation, Population: \n";
        printTrailCurrentPopulation();
        
        crossover();
        cout<<"\nAfter Crossover, Population: \n";
        printTrailCurrentPopulation();
        
        selection();
        cout<<"\nAfter Selection, Population: \n";
        printCurrentPopulation();
        
        calculateTotalFitness();

        currentGeneration++;
    }

    
    cout<<"**************************************************************\n";
    return 0;
}