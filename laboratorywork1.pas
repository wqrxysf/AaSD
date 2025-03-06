{
Москвичев А.Е. ПС-22
Pascal ABC
12. Имеется  текстовый   файл.    Разработать    программу,
обеспечивающую   вставку   запятой  перед  словами  "который",
"которая",  "которое",  "которые", если они не являются первым
словом предложения (7).
}

program InsertComma;

const
  TriggerWords: array[1..12] of string = ('который', 'которая', 'которое', 'которые', 'который,', 'которая,', 'которое,', 'которые,', 'который.', 'которая.', 'которое.', 'которые.');
var
  InputFile, OutputFile: Text;
  InputFileName, OutputFileName, Line, LineWithoutComma, LineWithComma: String;
  Symb: Char;
  ChN, Counter: Integer;
  IsLineEnd, IsLineComma: Boolean;

procedure ProcessLine(var TempLine: String; CharN: Integer; IsLineEnd: Boolean);
var
  NewLine, Word: string;
  WordEnd: Integer;
  IsFirstWord, IsLastComma: Boolean;
begin {ProcessLine}

  // инициализация
  NewLine := '';
  WordEnd := 1;
  IsFirstWord := True;
  IsLastComma := False;

  while WordEnd <= CharN do
  begin
    Word := '';
    while (WordEnd <= CharN) and (TempLine[WordEnd] <> ' ') // формируем слово, одновременно проверяем валидность строки
    do
      begin
        if (TempLine[WordEnd] = ',') then
          IsLastComma := True
        else
          Word := Word + TempLine[WordEnd];
        Inc(WordEnd);
      end;
    if WordEnd <= CharN then // проверяем первое слово
    begin
      if TempLine[WordEnd] = '.' then
        IsFirstWord := True;
    end;
    if Word <> '' then // обрабатываем полученное слово
    begin
      if not IsFirstWord then
      if (Word in TriggerWords) or (IsLastComma)  then
        NewLine := NewLine + ', ' + Word // триггерные слова
      else
        NewLine := NewLine + ' ' + Word // остальные случаи
      else
        NewLine := NewLine + Word; // для первого слова
      IsFirstWord := False;
    end;
    IsLastComma := False;
    Inc(WordEnd) // пропускаем пробел после слова
  end;
  TempLine := NewLine
end; {ProcessLine}

begin {InsertComma}
  // пользовательский ввод:
  Write('Enter the name of the input file in .txt format: ');
  ReadLn(InputFileName);
  
  AssignFile(InputFile, InputFileName);
  try // обработка ошибки открытия файла
    Reset(InputFile);
  except
    WriteLn('FATAL ERROR! The file with the entered name does not exist or is outside the program directory');
    WriteLn('The program performed a crash.');
    exit
  end;
 
  Write('Enter the name of the output file in any format: ');
  ReadLn(OutputFileName);
  AssignFile(OutputFile, OutputFileName);
  Rewrite(OutputFile);
  
  Counter := 0;
  while not Eof(InputFile) do
  begin
    ChN := 0;
    Line := '';
    IsLineComma := False;
    while not Eoln(InputFile) do // подсчёт количества символов в строке
    begin
      Inc(ChN);
      Read(InputFile, Symb);
      Line := Line + Symb;
      if (Line in TriggerWords) and (Counter <> 0) and (Counter <> 2) then
      begin
        WriteLn(OutputFile, LineWithComma);
        Counter := 2
      end
      else
        IsLineComma := False;
    end;
    if (not(IsLineComma)) and (Counter <> 0) and (Counter <> 2) then
      WriteLn(OutputFile, LineWithoutComma);
    if (Symb <> '.') and (Symb <> ',') and (Symb <> '!') and (Symb <> '?') and (Symb <> ':') then
      IsLineEnd := False
    else
      IsLineEnd := True;
    ReadLn(InputFile); // Переход на новую строку
    ProcessLine(Line, ChN, IsLineEnd);
    LineWithoutComma := Line;
    LineWithComma := Line + ',';
    Counter := 1
  end;

  WriteLn(OutputFile, LineWithoutComma);
  CloseFile(InputFile);
  CloseFile(OutputFile);
  WriteLn('Processing complete. The output has been written to ', OutputFileName)
  
end. {InsertComma}
