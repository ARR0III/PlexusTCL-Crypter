#ifdef PTCL_RUSSIAN_LANGUAGE
const char * STR_PROGRAMM_NAME          = "PlexusTCL Crypter 5.14 31MAY25 [RU]";

const char * STR_ERASED_FILE_OK         = "���� ���������� ��� ���������!";
const char * STR_ENTER_8_TO_256         = "������� ����� �� 8 �� 256!";
const char * STR_MEMORY_BLOCKED         = "������ ��������� ������!";
const char * STR_WARNING_MSG            = "��������!";
const char * STR_ERROR_MSG              = "������!";

const char * STR_INPUT_FILENAME         = "���� ��� ����������";
const char * STR_OUTPUT_FILENAME        = "���� ����������";
const char * STR_KEY_FILENAME           = "���� ����������";

const char * STR_ERASED                 = "�������";
const char * STR_START                  = "�����";
const char * STR_STOP                   = "����";
const char * STR_OPEN                   = "�������";
const char * STR_CLOSE                  = "�������";
const char * STR_KEYGEN                 = "���������";

const char * STR_EN                     = "�����������";
const char * STR_DE                     = "������������";

const char * STR_CRYPT_ALG              = "�������� ����������";
const char * STR_KEY_LENGTH             = "����� ����� ����������";
const char * STR_STATUS                 = "���������...";
const char * STR_KEY_OR_KEY_FILENAME    = "������ ��� ���� � ��������� �����";

const char * STR_ERASED_FILE_PROC       = "����������� �����; ����������: ";
const char * STR_ENCRYPT_FILE_PROC      = "%s: %s; ����������: ";
const char * STR_PROGRESS_BAR_MKS       = "%4.2f %s / %4.2f %s; ��������: %3d %%";

const char * STR_STOP_PROCESS           = "�������� ��������?";
const char * STR_INPUT_FILENAME_EMPTY   = "��� ��������������� ����� �� �������!";
const char * STR_OUTPUT_FILENAME_EMPTY  = "��� ����� ���������� �� �������!";
const char * STR_PASS_OR_KEYFILE_EMPTY  = "������ ��� ��� ��������� ����� �� �������!";
const char * STR_IN_OUT_FILENAME_EQUAL  = "����� ��������������� ����� � ����� ���������� ���������!";
const char * STR_OUT_KEY_FILENAME_EQUAL = "����� ��������������� ����� � ��������� ����� ���������!";
const char * STR_IN_KEY_FILENAME_EQUAL  = "����� ����� ���������� � ��������� ����� ���������!";
const char * STR_IN_FILENAME_NFOUND     = "���� ��� ��������� �� ����������!";
const char * STR_CHANGE_OUT_FILENAME    = "�������� ��� ����� ����������!";

const char * STR_CIPHER_NOT_ENTER       = "�������� ���������� �� ��� ������!";
const char * STR_KEY_SIZE_NOT_ENTER     = "����� ����� ���������� �� ���� �������!";
const char * STR_OPERATION_NOT_ENTER    = "�������� �� ���� �������!";

const char * STR_OK                          = "���� ������� ���������!";
const char * STR_READ_FILE_NOT_OPEN          = "���� ��� ��������� �� ��� ������!";
const char * STR_WRITE_FILE_NOT_OPEN         = "���� ���������� �� ��� ������!";
const char * STR_READ_FILE_ERROR             = "������ ������ �� �����!";
const char * STR_WRITE_FILE_ERROR            = "������ ������ � ����!";
const char * STR_STREAM_IN_CLOSE_ERROR       = "����� ����� �� ��� ������!";
const char * STR_STREAM_OUT_CLOSE_ERROR      = "����� ������ �� ��� ������!";
const char * STR_SIZE_FILE_EMPTY             = "���� ��� ��������� ����!";
const char * STR_SIZE_FILE_VERY_BIG          = "������ ����� ��� ��������� ��������� 4 ���!";
const char * STR_OPERATION_BREAK             = "�������� ���� ��������!";

const char * STR_SIZE_DECRYPT_FILE_INCORRECT =
  "������ ����� ��� ����������� �����������!\n"
  "�������������� ���� ����� ��� ����������?";
const char * STR_ERASED_FILE_QUES =
  "�� ������� ��� ������ ���������� ���� ��� ���������?\n"
  "������� ������ ����� ���������� ������������!";
const char * STR_ERROR_ERASED_FILE =
  "������ ���������� �����!\n"
  "�������� ���� �� ���������� ��� ������� �� ������!";
const char * STR_OUT_FILE_FOUND =
  "���� ���������� ����������! ������ ������ ����� �������!\n"
  "�� ������� ��� ������ ������������ ���?";
const char * STR_CONTROL_SUM_FILE_ERROR = 
  "����������� ����� ����� �� ��������� � ���������!\n"
  "�������� ���� ��������� ��� ��� ����������� ������������ ����!";
#else
const char * STR_PROGRAMM_NAME = "PlexusTCL Crypter 5.14 31MAY25 [RU]";

const char * STR_ERASED_FILE_OK = "The destination file was destroyed!";
const char * STR_ENTER_8_TO_256 = "Enter a number between 8 and 256!";
const char * STR_MEMORY_BLOCKED = "Memory allocation error!";
const char * STR_WARNING_MSG = "Attention!";
const char * STR_ERROR_MSG = "Error!";

const char * STR_INPUT_FILENAME  = "Source file";
const char * STR_OUTPUT_FILENAME = "Destination file";
const char * STR_KEY_FILENAME = "Encryption key";

const char * STR_ERASED = "Erase";
const char * STR_START = "Start";
const char * STR_STOP = "Stop";
const char * STR_OPEN = "Open";
const char * STR_CLOSE = "Close";
const char * STR_KEYGEN = "Generator";

const char * STR_EN = "Encrypt";
const char * STR_DE = "Decrypt";

const char * STR_CRYPT_ALG = "Encryption algorithm";
const char * STR_KEY_LENGTH = "Encryption key length";
const char * STR_STATUS = "Status...";
const char * STR_KEY_OR_KEY_FILENAME = "Password or path to key file";

const char * STR_ERASED_FILE_PROC = "File erasure; Processed: ";
const char * STR_ENCRYPT_FILE_PROC = "%s: %s; Processed: ";
const char * STR_PROGRESS_BAR_MKS = "%4.2f %s / %4.2f %s; Progress: %3d %%";

const char * STR_STOP_PROCESS = "Abort operation?";
const char * STR_INPUT_FILENAME_EMPTY = "No file name entered for the file being processed!";
const char * STR_OUTPUT_FILENAME_EMPTY = "No file name entered for the destination file!";
const char * STR_PASS_OR_KEYFILE_EMPTY = "Password or keyfile name not entered!";
const char * STR_IN_OUT_FILENAME_EQUAL = "The names of the file being processed and the destination file are the same!";
const char * STR_OUT_KEY_FILENAME_EQUAL = "The names of the file being processed and the keyfile are the same!";
const char * STR_IN_KEY_FILENAME_EQUAL = "The names of the destination file and the keyfile are the same!";
const char * STR_IN_FILENAME_NFOUND = "The file to be processed does not exist!";
const char * STR_CHANGE_OUT_FILENAME = "Change the name of the destination file!";

const char * STR_CIPHER_NOT_ENTER = "No encryption algorithm has been selected!";
const char * STR_KEY_SIZE_NOT_ENTER = "Encryption key length was not selected!";
const char * STR_OPERATION_NOT_ENTER = "Operation was not selected!";

const char * STR_OK = "File processed successfully!";
const char * STR_READ_FILE_NOT_OPEN = "File for processing was not open!";
const char * STR_WRITE_FILE_NOT_OPEN = "Destination file was not open!";
const char * STR_READ_FILE_ERROR = "Error reading from file!";
const char * STR_WRITE_FILE_ERROR = "Error writing to file!";
const char * STR_STREAM_IN_CLOSE_ERROR = "Input stream was not closed!";
const char * STR_STREAM_OUT_CLOSE_ERROR = "The output stream was not closed!";
const char * STR_SIZE_FILE_EMPTY = "The file to process is empty!";
const char * STR_SIZE_FILE_VERY_BIG = "The size of the file to process exceeds 4 EiB!";
const char * STR_OPERATION_BREAK = "The operation was canceled!";

const char * STR_SIZE_DECRYPT_FILE_INCORRECT =
  "The size of the file to decrypt is incorrect!\n"
  "Was the file to be processed previously encrypted?";
const char * STR_ERASED_FILE_QUES =
  "Are you sure you want to delete the file to process?\n"
  "Erased data cannot be recovered!";
const char * STR_ERROR_ERASED_FILE =
  "Error overwriting file!\n"
  "The file may not exist or is write-protected!";
const char * STR_OUT_FILE_FOUND =
  "The destination file exists! Old data will be lost!\n"
  "Are you sure you want to overwrite it?";
const char * STR_CONTROL_SUM_FILE_ERROR =
  "The file checksum does NOT match the expected one!\n"
  "The file may be corrupted or the wrong key was used!";
#endif

