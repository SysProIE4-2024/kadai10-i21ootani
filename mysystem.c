/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース
char *execpath = "/bin/sh";

// system関数のクローン
int mysystem(char *command) {
  int pid;
  int status = 0;
  if (command == NULL) {
    return 1;
  }
  if ((pid = fork()) < 0) {
    perror("fork");
    return -1;
  }
  if (pid != 0) {
    int r;
    while ((r = wait(&status)) != pid) {
      if (r < 0) {
        return -1;
      }
    }
  } else {
    execl(execpath, "sh", "-c", command, NULL);
    exit(127);
  }
  return status;
}

/* 実行例
% make                 <- エラーも警告も出ない
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c
% ./mysysmain          <- コマンドを書かなかった場合
使い方 : ./mysysmain コマンド文字
% ./mysysmain date     <- コマンドを書いた場合
mysystem:
2024年 7月 8日 月曜日 23時38分52秒 JST
retval = 00000000
system:
2024年 7月 8日 月曜日 23時38分52秒 JST
retval = 00000000
% ./mysysmain ls -l    <- コマンド行引数が2つより多い場合
使い方 : ./mysysmain コマンド文字
% ./mysysmain "ls -l"  <- コマンド行引数が2つより多くなるときに
                          ダブルクオーテーションで囲んだ場合
mysystem:
total 616
-rw-r--r--  1 ootaniyuuki  staff     143  7  4 09:44 Makefile
-rw-r--r--  1 ootaniyuuki  staff    2795  7  4 09:44 README.md
-rw-r--r--@ 1 ootaniyuuki  staff  238232  7  4 09:44 README.pdf
-rwxr-xr-x  1 ootaniyuuki  staff   50392  7  8 23:37 mysysmain
-rw-r--r--  1 ootaniyuuki  staff     925  7  8 23:37 mysysmain.c
-rw-r--r--@ 1 ootaniyuuki  staff     694  7  8 23:35 mysystem.c
-rw-r--r--  1 ootaniyuuki  staff      90  7  4 09:44 mysystem.h
retval = 00000000
system:
total 616
-rw-r--r--  1 ootaniyuuki  staff     143  7  4 09:44 Makefile
-rw-r--r--  1 ootaniyuuki  staff    2795  7  4 09:44 README.md
-rw-r--r--@ 1 ootaniyuuki  staff  238232  7  4 09:44 README.pdf
-rwxr-xr-x  1 ootaniyuuki  staff   50392  7  8 23:37 mysysmain
-rw-r--r--  1 ootaniyuuki  staff     925  7  8 23:37 mysysmain.c
-rw-r--r--@ 1 ootaniyuuki  staff     694  7  8 23:35 mysystem.c
-rw-r--r--  1 ootaniyuuki  staff      90  7  4 09:44 mysystem.h
retval = 00000000
% ./mysysmain a        <- 存在しないコマンドを書いた場合
mysystem:
sh: a: command not found
retval = 00007f00
system:
sh: a: command not found
retval = 00007f00
% ./mysysmain "date a" <- コマンドの引数が違う場合
mysystem:
date: illegal time format
usage: date [-jnRu] [-I[date|hours|minutes|seconds]] [-f input_fmt]
            [-r filename|seconds] [-v[+|-]val[y|m|w|d|H|M|S]]
            [[[[mm]dd]HH]MM[[cc]yy][.SS] | new_date] [+output_fmt]
retval = 00000100
system:
date: illegal time format
usage: date [-jnRu] [-I[date|hours|minutes|seconds]] [-f input_fmt]
            [-r filename|seconds] [-v[+|-]val[y|m|w|d|H|M|S]]
            [[[[mm]dd]HH]MM[[cc]yy][.SS] | new_date] [+output_fmt]
retval = 00000100
*/
