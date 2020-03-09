// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    BuildValidWords();
   
    SetUpGame();       
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
   if(bGameOver){
        ClearScreen();
        SetUpGame();
   }else {
       ProcessGuess(Input);        
   }    
}

void UBullCowCartridge::SetUpGame(){
    //Welcome the player
    PrintLine(TEXT("Welcome to Bulls & Cows."));
   
    int32 selectedIndex = FMath::RandRange(0,Isograms.Num()-1);
    HiddenWord = Isograms[selectedIndex];
    Lives=HiddenWord.Len();

    PrintLine(TEXT("HiddenWord %s"), *HiddenWord);  

    PrintLine(TEXT("In this game you have to guess a %i \n character long isogram word"), HiddenWord.Len());
    PrintLine(TEXT("Press Tab to get into the terminal"));
    PrintLine(TEXT("Type your guess \n Press Enter tocontinue\nYou have %i lives"), Lives);    
    
    bGameOver=false;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess){
    if(HiddenWord==Guess){
            PrintLine(TEXT("You guessed the word!! Mooooooooo!"));
            EndGame();
            return;
    }

    if(HiddenWord.Len() != Guess.Len()){    
        PrintLine(TEXT("The word has to be %i characters long"), HiddenWord.Len());
        PrintLine(TEXT("Try again!"));
        return;        
    }

    if(!IsIsogram(Guess)){
        PrintLine(TEXT("The word has to be an isogram"));
        PrintLine(TEXT("Try again!"));
        return;
    }

    PrintLine(TEXT("Wrong answer! You have %i lives left"),--Lives);
    if(Lives<=0){
            ClearScreen();
            PrintLine(TEXT("You've lost! The cows will eat you now!"));
            PrintLine(TEXT("\nThe hidden word was %s"), *HiddenWord);
            EndGame();
    }

    //Show bulls and cows
    int32 Bulls, Cows;
    GetBullCows(Guess,Bulls,Cows);

    PrintLine(TEXT("You have %i bulls and %i cows"), Bulls, Cows);       
    
}

void UBullCowCartridge::EndGame(){
    bGameOver=true;    
    PrintLine(TEXT("\n\nPress Enter to continue"));    
}

void UBullCowCartridge::BuildValidWords(){    
     for(FString CurrentWord : Words){
        if(CurrentWord.Len() >=4 && CurrentWord.Len()<=8 && IsIsogram(CurrentWord)){
            Isograms.Emplace(CurrentWord);
        }
    }
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const{
    BullCount = 0;
    CowCount = 0;
    for (int32 Index = 0; Index < Guess.Len(); Index++){
        if(Guess[Index] == HiddenWord[Index]){
            BullCount++;
            continue;
        }
        for (int32 Jindex = 0; Jindex < HiddenWord.Len(); Jindex++){
            if(Guess[Index] == HiddenWord[Jindex]){
                CowCount++;
                continue;
            }
        }
        
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const{  

    for(int32 Index=0; Index < Word.Len(); Index++){
        for(int32 Comparison = Index+1; Comparison < Word.Len(); Comparison++){
            if(Word[Index]==Word[Comparison]){
                return false;
            }
        }
    }

    return true;
    /*
        Esta solucion funciona, sin embargo...
        Estamos volviendo a checar valores que ya hemos revisado, es decir que despues de revisar el index 0 contra la posicion 1,2,3 (en el caso de un arreglo de 4),
        cuando la i pasa al siguiente y toma valor de 1, volvemos a verificar 1 con 0 (solo que ahora al reves)
    */  
    // bool IsIsogram = true;
    // for(int32 i=0; i<CurrentWord.Len(); i++){
    //     char PivotLetter = CurrentWord[i];
    //     for(int32 j=0; j<CurrentWord.Len();j++){
    //         //PrintLine(TEXT("Comparando %c con %c"), CurrentWord[i], CurrentWord[j]);
    //         if(i!=j){
    //             if (CurrentWord[i]==CurrentWord[j]){
    //                 IsIsogram=false;
    //                 break;
    //             }
    //         }
    //     }
    //     if(!IsIsogram){
    //         break;
    //     }
    //}
    
    //return IsIsogram;
}