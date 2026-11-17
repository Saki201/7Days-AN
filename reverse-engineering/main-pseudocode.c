/* Address: 0001afe8 */

/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined4 FUN_0001afe8(int param_1,int param_2)

{
  uint *puVar1;
  undefined2 uVar2;
  undefined1 auVar3 [16];
  longlong lVar4;
  longlong lVar5;
  ulonglong uVar6;
  int iVar7;
  byte bVar8;
  size_t sVar9;
  int iVar10;
  int *piVar11;
  undefined4 extraout_r0;
  undefined4 extraout_r0_00;
  undefined4 extraout_r0_01;
  undefined4 extraout_r0_02;
  size_t *psVar12;
  int iVar13;
  int iVar14;
  undefined4 extraout_r0_03;
  uint uVar15;
  undefined4 *puVar16;
  undefined4 extraout_r0_04;
  void *pvVar17;
  undefined4 extraout_r0_05;
  undefined4 extraout_r1;
  undefined4 extraout_r1_00;
  undefined4 extraout_r1_01;
  undefined4 extraout_r1_02;
  size_t sVar18;
  int iVar19;
  undefined4 extraout_r1_03;
  undefined4 extraout_r1_04;
  undefined4 extraout_r1_05;
  undefined4 extraout_r1_06;
  void *__s2;
  void *__s2_00;
  uint **ppuVar20;
  size_t sVar21;
  uint *puVar22;
  int iVar23;
  undefined4 uVar24;
  char *pcVar25;
  uint uVar26;
  void *__s2_01;
  int iVar27;
  size_t *psVar28;
  bool bVar29;
  double dVar30;
  double dVar31;
  undefined8 uVar32;
  undefined8 uVar33;
  longlong lVar34;
  longlong lVar35;
  longlong lVar36;
  uint **local_308;
  uint **local_304;
  int local_300;
  uint local_2fc;
  int local_2f8;
  int local_2f4;
  int local_2cc;
  int local_2c8;
  int local_2c4;
  int *local_2c0;
  int *local_2bc;
  int *local_2b8;
  size_t *local_2b0;
  undefined4 uStack_2ac;
  size_t local_2a8;
  undefined4 uStack_2a4;
  undefined8 local_2a0;
  size_t *local_298;
  size_t local_294;
  size_t local_290 [4];
  uint *local_280 [2];
  uint local_278;
  undefined1 local_274 [12];
  char *local_268;
  undefined4 local_264;
  undefined4 local_260;
  undefined8 local_25c;
  undefined8 uStack_254;
  undefined8 local_24c;
  undefined4 local_244;
  char *local_238;
  char *local_220;
  char *local_208;
  char *local_1f0;
  uint *local_1d8;
  size_t local_1d4;
  uint local_1d0 [4];
  uint *local_1c0;
  size_t local_1bc;
  uint local_1b8 [4];
  undefined1 *local_1a8;
  undefined4 local_1a4;
  undefined1 local_1a0 [16];
  undefined8 local_190;
  int local_188;
  undefined8 local_184;
  undefined1 *local_17c;
  undefined4 local_178;
  undefined1 local_174 [16];
  float local_164;
  undefined8 local_160;
  int local_158;
  undefined8 local_154;
  float local_14c;
  undefined2 local_148;
  undefined8 local_144;
  undefined8 uStack_13c;
  undefined8 local_134;
  undefined4 local_12c;
  undefined1 *local_128;
  int local_124;
  undefined1 local_120 [16];
  undefined8 local_110;
  uint *local_108;
  size_t local_104;
  uint local_100 [4];
  int local_f0;
  char local_e3;
  
  local_154 = 0;
  local_14c = 0.0;
  local_190 = 0;
  local_164 = (float)((ulonglong)DAT_0001b1a8 >> 0x20);
  local_184 = DAT_0001b1a8;
  local_148 = 0;
  local_1a8 = local_1a0;
  local_1d4 = 0;
  local_1d0[0] = local_1d0[0] & 0xffffff00;
  local_1bc = 0;
  local_1b8[0] = local_1b8[0] & 0xffffff00;
  local_1a4 = 0;
  local_1a0[0] = 0;
  local_188 = 0;
  local_178 = 0;
  local_174[0] = 0;
  local_124 = 0;
  local_120[0] = 0;
  local_12c = 1;
  local_2c4 = 1;
  local_110 = DAT_0001b1b0;
  local_144 = 0;
  uStack_13c = 0;
  local_17c = local_174;
  local_128 = local_120;
  local_158 = -1;
  local_2cc = param_1;
  local_2c8 = param_2;
  local_1d8 = local_1d0;
  local_1c0 = local_1b8;
  local_160 = local_190;
  local_134 = local_154;
  if (1 < param_1) {
    __s2 = (void *)(DAT_0001b1b8 + 0x1b0dc);
    __s2_01 = (void *)(DAT_0001b1bc + 0x1b0e4);
    __s2_00 = (void *)(DAT_0001b1c0 + 0x1b0ec);
    do {
      pcVar25 = *(char **)(local_2c8 + local_2c4 * 4);
      local_298 = local_290;
      if (pcVar25 == (char *)0x0) {
                    /* WARNING: Subroutine does not return */
        FUN_00019810(&UNK_0001c3bc + DAT_0001bf2c);
      }
      sVar9 = strlen(pcVar25);
      if (sVar9 < 0x10) {
        psVar12 = local_290;
        if (sVar9 == 1) {
          local_290[0] = CONCAT31(local_290[0]._1_3_,*pcVar25);
        }
        else if (sVar9 != 0) goto LAB_0001b54c;
      }
      else {
        local_298 = (size_t *)FUN_002be2e4(sVar9 + 1);
        psVar12 = local_298;
        local_290[0] = sVar9;
LAB_0001b54c:
        memcpy(psVar12,pcVar25,sVar9);
        psVar12 = local_298;
      }
      psVar28 = local_298;
      *(undefined1 *)((int)psVar12 + sVar9) = 0;
      local_2c0 = &local_2c4;
      local_2bc = &local_2cc;
      local_2b8 = &local_2c8;
      local_294 = sVar9;
      if (sVar9 == 6) {
        iVar10 = memcmp(local_298,__s2_01,6);
        if (iVar10 == 0) {
          iVar10 = local_2c4 + 1;
          if (iVar10 < local_2cc) {
            pcVar25 = *(char **)(local_2c8 + iVar10 * 4);
            local_2c4 = iVar10;
            local_108 = local_100;
            if (pcVar25 == (char *)0x0) {
                    /* WARNING: Subroutine does not return */
              FUN_00019810(&UNK_0001c388 + DAT_0001bf28);
            }
            sVar9 = strlen(pcVar25);
            if (sVar9 < 0x10) {
              if (sVar9 == 1) {
                local_100[0] = CONCAT31(local_100[0]._1_3_,*pcVar25);
              }
              else {
                puVar22 = local_100;
                if (sVar9 != 0) goto LAB_0001ba84;
              }
            }
            else {
              local_108 = (uint *)FUN_002be2e4(sVar9 + 1);
              puVar22 = local_108;
              local_100[0] = sVar9;
LAB_0001ba84:
              memcpy(puVar22,pcVar25,sVar9);
            }
            uVar26 = local_1d0[0];
            puVar22 = local_1d8;
            *(undefined1 *)((int)local_108 + sVar9) = 0;
            local_104 = sVar9;
            if (local_1d8 == local_1d0) goto LAB_0001b630;
            if (local_108 == local_100) goto LAB_0001ba94;
            local_1d8 = local_108;
            local_1d0[0] = local_100[0];
            local_1d4 = sVar9;
            local_108 = puVar22;
            if (puVar22 != (uint *)0x0) goto LAB_0001ba24;
LAB_0001b650:
            local_108 = local_100;
            uVar26 = local_100[0];
          }
          else {
            local_108 = local_100;
            local_100[0] = local_100[0] & 0xffffff00;
            local_104 = 0;
            if (local_1d8 == local_1d0) {
LAB_0001b630:
              if (local_108 != local_100) {
                local_1d8 = local_108;
                local_1d4 = local_104;
                local_1d0[0] = local_100[0];
                goto LAB_0001b650;
              }
            }
LAB_0001ba94:
            if (local_104 != 0) {
              if (local_104 == 1) {
                *(char *)local_1d8 = (char)local_100[0];
              }
              else {
                memcpy(local_1d8,local_100,local_104);
              }
            }
            *(undefined1 *)((int)local_1d8 + local_104) = 0;
            local_1d4 = local_104;
            uVar26 = local_100[0];
          }
LAB_0001ba24:
          local_100[0] = uVar26;
          *(undefined1 *)local_108 = 0;
          if (local_108 != local_100) {
            free(local_108);
          }
        }
        else {
          iVar10 = memcmp(psVar28,(void *)(DAT_0001b1fc + 0x1b590),6);
          if (iVar10 != 0) goto LAB_0001b250;
          FUN_000e6c2c(&local_108,&local_2c0);
          iVar10 = atoi((char *)local_108);
          local_190 = CONCAT44(iVar10,(undefined4)local_190);
          FUN_000210c8(&local_108);
        }
      }
      else if (sVar9 == 5) {
        iVar10 = memcmp(local_298,__s2,5);
        if (iVar10 == 0) {
          iVar10 = local_2c4 + 1;
          if (iVar10 < local_2cc) {
            pcVar25 = *(char **)(local_2c8 + iVar10 * 4);
            local_2c4 = iVar10;
            local_108 = local_100;
            if (pcVar25 == (char *)0x0) {
                    /* WARNING: Subroutine does not return */
              FUN_00019810(&UNK_0001c404 + DAT_0001bf34);
            }
            sVar9 = strlen(pcVar25);
            if (sVar9 < 0x10) {
              if (sVar9 == 1) {
                local_100[0] = CONCAT31(local_100[0]._1_3_,*pcVar25);
              }
              else {
                puVar22 = local_100;
                if (sVar9 != 0) goto LAB_0001c0c8;
              }
            }
            else {
              local_108 = (uint *)FUN_002be2e4(sVar9 + 1);
              puVar22 = local_108;
              local_100[0] = sVar9;
LAB_0001c0c8:
              memcpy(puVar22,pcVar25,sVar9);
            }
            uVar26 = local_1b8[0];
            puVar22 = local_1c0;
            *(undefined1 *)((int)local_108 + sVar9) = 0;
            local_104 = sVar9;
            if (local_1c0 == local_1b8) goto LAB_0001b6b8;
            if (local_108 == local_100) goto LAB_0001be9c;
            bVar29 = local_1c0 == (uint *)0x0;
            local_1b8[0] = local_100[0];
            local_1c0 = local_108;
            local_1bc = sVar9;
            local_108 = puVar22;
            if (bVar29) goto LAB_0001b650;
          }
          else {
            local_108 = local_100;
            local_100[0] = local_100[0] & 0xffffff00;
            local_104 = 0;
            if (local_1c0 == local_1b8) {
LAB_0001b6b8:
              if (local_108 != local_100) {
                local_1b8[0] = local_100[0];
                local_1c0 = local_108;
                local_1bc = local_104;
                goto LAB_0001b650;
              }
LAB_0001be9c:
              if (local_104 != 0) {
                if (local_104 == 1) {
                  *(char *)local_1c0 = (char)local_100[0];
                }
                else {
                  memcpy(local_1c0,local_100,local_104);
                }
              }
            }
            *(undefined1 *)((int)local_1c0 + local_104) = 0;
            local_1bc = local_104;
            uVar26 = local_100[0];
          }
          goto LAB_0001ba24;
        }
        iVar10 = memcmp(local_298,__s2_00,5);
        if (iVar10 != 0) goto LAB_0001b250;
        FUN_000e6c2c(&local_108,&local_2c0);
        iVar10 = atoi((char *)local_108);
        local_190 = CONCAT44(local_190._4_4_,iVar10);
        FUN_000210c8(&local_108);
      }
      else if ((sVar9 == 7) &&
              (iVar10 = memcmp(local_298,(void *)(DAT_0001b1c4 + 0x1b178),7), iVar10 == 0)) {
        FUN_000e6c2c(&local_108,&local_2c0);
        local_188 = atoi((char *)local_108);
        FUN_000210c8(&local_108);
      }
      else {
LAB_0001b250:
        iVar10 = FUN_0007c96c(&local_298,DAT_0001b1c8 + 0x1b260);
        if (iVar10 == 0) {
          iVar10 = FUN_0007c96c(&local_298,DAT_0001b1cc + 0x1b27c);
          if (iVar10 == 0) {
            iVar10 = FUN_0007c96c(&local_298,DAT_0001b1d0 + 0x1b294);
            if (iVar10 == 0) {
              iVar10 = FUN_0007c96c(&local_298,DAT_0001b1d4 + 0x1b2ac);
              if (iVar10 == 0) {
                iVar10 = FUN_0007c96c(&local_298,DAT_0001b1d8 + 0x1b2c4);
                if (iVar10 == 0) {
                  iVar10 = FUN_0007c96c(&local_298,DAT_0001b1dc + 0x1b2e4);
                  if (iVar10 == 0) {
                    iVar10 = FUN_0007c96c(&local_298,DAT_0001b1e0 + 0x1b304);
                    if (iVar10 == 0) {
                      iVar10 = FUN_0007c96c(&local_298,DAT_0001b1e4 + 0x1b324);
                      if (iVar10 == 0) {
                        iVar10 = FUN_0007c96c(&local_298,DAT_0001b1e8 + 0x1b344);
                        if (iVar10 == 0) {
                          iVar10 = FUN_0007c96c(&local_298,DAT_0001b1ec + 0x1b35c);
                          if (iVar10 == 0) {
                            iVar10 = FUN_0007c96c(&local_298,DAT_0001b1f0 + 0x1b37c);
                            if (iVar10 == 0) {
                              iVar10 = FUN_0007c96c(&local_298,DAT_0001b1f4 + 0x1b394);
                              if (iVar10 == 0) {
                                iVar10 = FUN_0007c96c(&local_298,DAT_0001b1f8 + 0x1b3ac);
                                if (iVar10 != 0) {
                                  FUN_000e6c2c(local_280,&local_2c0);
                                  FUN_000e6d3c(&local_108,local_280,0x18);
                                  FUN_000210c8(local_280);
                                  iVar10 = 7;
                                  ppuVar20 = local_280;
                                  do {
                                    iVar10 = iVar10 + -1;
                                    ppuVar20[1] = (uint *)0x0;
                                    *(undefined1 *)(ppuVar20 + 2) = 0;
                                    *ppuVar20 = (uint *)(ppuVar20 + 2);
                                    ppuVar20 = ppuVar20 + 6;
                                  } while (iVar10 != 0);
                                  iVar10 = 0;
                                  local_304 = local_280;
                                  do {
                                    piVar11 = (int *)FUN_002bebb0(&local_108,local_304,0x2c);
                                    if ((*(uint *)((int)piVar11 + *(int *)(*piVar11 + -0xc) + 0x14)
                                        & 5) != 0) {
                                      iVar13 = local_158;
                                      if (iVar10 != 6) goto LAB_0001b50c;
                                      iVar13 = 6;
                                      goto LAB_0001b470;
                                    }
                                    iVar10 = iVar10 + 1;
                                    local_304 = local_304 + 6;
                                  } while (iVar10 != 7);
                                  iVar13 = 7;
LAB_0001b470:
                                  atof((char *)local_280[0]);
                                  local_164 = (float)(double)CONCAT44(extraout_r1,extraout_r0);
                                  iVar10 = atoi(local_268);
                                  local_160 = CONCAT44(local_160._4_4_,iVar10);
                                  iVar10 = atoi(uStack_254._4_4_);
                                  local_160 = CONCAT44(iVar10,(undefined4)local_160);
                                  atof(local_238);
                                  atof(local_220);
                                  atof(local_208);
                                  bVar29 = iVar13 != 7;
                                  if (bVar29) {
                                    iVar13 = -1;
                                  }
                                  local_154 = CONCAT44((float)(double)CONCAT44(extraout_r1_01,
                                                                               extraout_r0_01),
                                                       (float)(double)CONCAT44(extraout_r1_00,
                                                                               extraout_r0_00));
                                  local_14c = (float)(double)CONCAT44(extraout_r1_02,extraout_r0_02)
                                  ;
                                  if (!bVar29) {
                                    iVar13 = atoi(local_1f0);
                                  }
LAB_0001b50c:
                                  local_158 = iVar13;
                                  iVar10 = 6;
                                  do {
                                    iVar13 = iVar10 * 6;
                                    iVar10 = iVar10 + -1;
                                    FUN_000210c8(local_280 + iVar13);
                                  } while (iVar10 != -1);
                                  FUN_003171bc(&local_108);
                                }
                              }
                              else {
                                local_304 = &local_2b0;
                                local_2a0 = 0;
                                local_2b0 = (size_t *)0x0;
                                uStack_2ac = 0;
                                local_2a8 = 0;
                                uStack_2a4 = 0;
                                FUN_000e6c2c(local_280,&local_2c0);
                                FUN_000e6d3c(&local_108,local_280,0x18);
                                FUN_000210c8(local_280);
                                local_300 = 6;
                                local_280[1] = (uint *)0x0;
                                local_278 = local_278 & 0xffffff00;
                                local_280[0] = &local_278;
                                do {
                                  piVar11 = (int *)FUN_002bebb0(&local_108,local_280,0x2c);
                                  if ((*(uint *)((int)piVar11 + *(int *)(*piVar11 + -0xc) + 0x14) &
                                      5) != 0) break;
                                  atof((char *)local_280[0]);
                                  local_300 = local_300 + -1;
                                  *local_304 = (size_t *)
                                               (float)(double)CONCAT44(extraout_r1_06,extraout_r0_05
                                                                      );
                                  local_304 = local_304 + 1;
                                } while (local_300 != 0);
                                local_144 = CONCAT44(uStack_2ac,local_2b0);
                                auVar3._8_4_ = local_2a8;
                                auVar3._0_8_ = local_144;
                                auVar3._12_4_ = uStack_2a4;
                                local_148 = CONCAT11(1,(undefined1)local_148);
                                uStack_13c = auVar3._8_8_;
                                local_134 = local_2a0;
                                FUN_000210c8(local_280);
                                FUN_003171bc(&local_108);
                              }
                            }
                            else {
                              FUN_000e6c2c(&local_108,&local_2c0);
                              puVar22 = local_108;
                              if (((local_104 != 0) &&
                                  (pvVar17 = memchr(local_108,0x78,local_104),
                                  pvVar17 != (void *)0x0)) && ((int)pvVar17 - (int)puVar22 != -1)) {
                                iVar10 = atoi((char *)puVar22);
                                if (iVar10 < 0xa0) {
                                  iVar10 = 0xa0;
                                }
                                local_110 = CONCAT44(local_110._4_4_,iVar10);
                                iVar10 = atoi((char *)((int)puVar22 +
                                                      ((int)pvVar17 - (int)puVar22) + 1));
                                if (iVar10 < 0x78) {
                                  iVar10 = 0x78;
                                }
                                local_110 = CONCAT44(iVar10,(undefined4)local_110);
                              }
                              FUN_000210c8(&local_108);
                            }
                          }
                          else {
                            local_12c = CONCAT13(1,(undefined3)local_12c);
                          }
                        }
                        else {
                          FUN_000e6c2c(&local_108,&local_2c0);
                          FUN_000e6b4c(&local_128,&local_108);
                          FUN_000210c8(&local_108);
                        }
                      }
                      else {
                        local_12c._0_3_ = CONCAT12(1,(undefined2)local_12c);
                      }
                    }
                    else {
                      local_12c._0_2_ = CONCAT11(1,(undefined1)local_12c);
                    }
                  }
                  else {
                    local_12c = CONCAT31(local_12c._1_3_,1);
                  }
                }
                else {
                  local_148 = CONCAT11(local_148._1_1_,1);
                }
              }
              else {
                FUN_000e6c2c(&local_108,&local_2c0);
                FUN_000e6b4c(&local_1a8,&local_108);
                FUN_000210c8(&local_108);
              }
            }
            else {
              FUN_000e6c2c(&local_108,&local_2c0);
              atof((char *)local_108);
              local_184 = CONCAT44((float)(double)CONCAT44(extraout_r1_05,extraout_r0_04),
                                   (undefined4)local_184);
              FUN_000210c8(&local_108);
            }
          }
          else {
            FUN_000e6c2c(&local_108,&local_2c0);
            FUN_000e6b4c(&local_17c,&local_108);
            FUN_000210c8(&local_108);
          }
        }
        else {
          FUN_000e6c2c(&local_108,&local_2c0);
          atof((char *)local_108);
          local_184 = CONCAT44(local_184._4_4_,
                               (float)(double)CONCAT44(extraout_r1_04,extraout_r0_03));
          FUN_000210c8(&local_108);
        }
      }
      if (local_298 != local_290) {
        free(local_298);
      }
      local_2c4 = local_2c4 + 1;
    } while (local_2c4 < local_2cc);
  }
  if (local_12c._3_1_ != '\0') {
    thunk_FUN_0012684c(DAT_0001b21c + 0x1ba5c,DAT_0001b218 + 0x1ba58);
  }
  thunk_FUN_0012680c(DAT_0001b204 + 0x1b700,DAT_0001b200 + 0x1b6fc,2);
  thunk_FUN_0012684c(DAT_0001b20c + 0x1b714,DAT_0001b208 + 0x1b710);
  iVar10 = thunk_FUN_0012982c(0x2030);
  if (iVar10 == 0) {
    uVar24 = thunk_FUN_0012d50c();
    FUN_000326b0(DAT_0001bf1c + 0x1be68,uVar24);
    uVar24 = 1;
  }
  else {
    FUN_000326e8(&local_1d8,&local_1c0);
    local_2b0 = &local_2a8;
    psVar12 = (size_t *)FUN_002be2e4(0x17);
    iVar10 = DAT_0001b210;
    local_2a8 = 0x16;
    psVar28 = (size_t *)(DAT_0001b210 + 0x1b758);
    uStack_2ac = 0x16;
    *(undefined1 *)((int)psVar12 + 0x16) = 0;
    sVar18 = *(size_t *)(iVar10 + 0x1b75c);
    sVar21 = *(size_t *)(iVar10 + 0x1b760);
    sVar9 = *(size_t *)(iVar10 + 0x1b764);
    *psVar12 = *psVar28;
    psVar12[3] = sVar9;
    sVar9 = *(size_t *)(iVar10 + 0x1b768);
    uVar2 = *(undefined2 *)(iVar10 + 0x1b76c);
    psVar12[1] = sVar18;
    psVar12[2] = sVar21;
    psVar12[4] = sVar9;
    *(undefined2 *)(psVar12 + 5) = uVar2;
    local_2b0 = psVar12;
    if (local_124 == 0) {
      local_298 = local_290;
      local_298 = (size_t *)FUN_002be2e4(0x17);
      sVar9 = psVar12[3];
      sVar18 = psVar12[2];
      *local_298 = *psVar12;
      local_298[3] = sVar9;
      sVar21 = psVar12[1];
      sVar9 = psVar12[5];
      local_298[2] = sVar18;
      local_298[1] = sVar21;
      *(short *)(local_298 + 5) = (short)sVar9;
      local_290[0] = 0x16;
      local_294 = 0x16;
      sVar9 = psVar12[4];
      *(undefined1 *)((int)local_298 + 0x16) = *(undefined1 *)((int)psVar12 + 0x16);
      local_298[4] = sVar9;
    }
    else {
      local_280[0] = &local_278;
      local_280[1] = (uint *)0x0;
      local_278 = local_278 & 0xffffff00;
      local_280[0] = (uint *)FUN_002be2e4(0x1f);
      *(undefined1 *)local_280[0] = 0;
      local_278 = 0x1e;
      FUN_0002bee0(local_280,psVar12,0x16);
      FUN_0002bee0(local_280,DAT_0001bf20 + 0x1bf98,2);
      puVar16 = (undefined4 *)FUN_0002bee0(local_280,local_128,local_124);
      local_108 = local_100;
      puVar22 = puVar16 + 2;
      if ((uint *)*puVar16 == puVar22) {
        memcpy(local_108,puVar22,puVar16[1] + 1);
      }
      else {
        local_100[0] = puVar16[2];
        local_108 = (uint *)*puVar16;
      }
      local_104 = puVar16[1];
      puVar16[1] = 0;
      *puVar16 = puVar22;
      *(undefined1 *)(puVar16 + 2) = 0;
      if (local_104 == 0x7ffffffe) {
                    /* WARNING: Subroutine does not return */
        FUN_0001993c(&UNK_0001c3e8 + DAT_0001bf30);
      }
      puVar16 = (undefined4 *)FUN_0002c108(&local_108,DAT_0001bf24 + 0x1c008,1);
      local_298 = local_290;
      psVar12 = puVar16 + 2;
      if ((size_t *)*puVar16 == psVar12) {
        memcpy(local_298,psVar12,puVar16[1] + 1);
      }
      else {
        local_290[0] = puVar16[2];
        local_298 = (size_t *)*puVar16;
      }
      local_294 = puVar16[1];
      *puVar16 = psVar12;
      puVar16[1] = 0;
      *(undefined1 *)(puVar16 + 2) = 0;
      FUN_000210c8(&local_108);
      FUN_000210c8(local_280);
    }
    iVar10 = FUN_001069ac(local_298,(undefined4)local_110,local_110._4_4_,local_12c._2_1_);
    if (iVar10 == 0) {
      uVar24 = 1;
    }
    else {
      iVar10 = FUN_00106b5c();
      FUN_000e6368();
      FUN_000e57a0(local_12c._1_1_);
      thunk_FUN_00041cbc();
      FUN_00109dd8();
      FUN_00105dbc(local_12c & 0xff);
      FUN_000f0024();
      FUN_0002d2f0();
      FUN_000a3cc8();
      iVar13 = FUN_000d9a24();
      FUN_000acc14(iVar13 + 0x44,&local_1a8);
      FUN_000def64(iVar13,&local_190);
      local_2c8 = iVar10;
      FUN_000e6ab8(&local_2c8);
      local_25c = 0;
      uStack_254 = 0;
      local_24c = 0;
      local_280[1] = (uint *)local_274;
      local_280[0] = (uint *)((uint)local_280[0] & 0xffffff00);
      local_278 = 0;
      local_274[0] = 0;
      local_264 = 0;
      local_260 = 0;
      local_244 = 0;
      if (local_124 != 0) {
        thunk_FUN_000e484c();
        FUN_00075108(local_280,&local_128,iVar10);
      }
      local_308 = local_280;
      uVar32 = thunk_FUN_0012a5b4();
      iVar7 = DAT_0001b214;
      lVar5 = 0;
      puVar22 = (uint *)(DAT_0001b214 + 0x1b8f8);
      lVar34 = 0;
      lVar4 = 0;
      do {
        local_2f8 = (int)((ulonglong)lVar34 >> 0x20);
        local_2fc = (uint)lVar34;
        local_300 = (int)((ulonglong)lVar4 >> 0x20);
        local_304 = (uint **)lVar4;
        local_2f4 = (int)((ulonglong)lVar5 >> 0x20);
        uVar6 = (ulonglong)uVar32 >> 0x20;
        uVar26 = (uint)uVar32;
        iVar14 = 0;
        while (iVar27 = iVar14, iVar14 = thunk_FUN_00128960(&local_108), iVar14 != 0) {
          FUN_000ab374(iVar13 + 0x44,&local_108);
          if (((local_108 != (uint *)0x100) && (iVar14 = iVar27, local_108 == (uint *)0x300)) &&
             (local_e3 == '\0')) {
            if (local_f0 == 0x44) {
              if (iVar10 != 0) {
                uVar15 = thunk_FUN_0012a37c(iVar10);
                thunk_FUN_00125ad4(iVar10,~uVar15 & 1);
              }
            }
            else if (local_f0 == 0x43) {
              bVar8 = FUN_00105dd0();
              FUN_00105dbc(bVar8 ^ 1);
            }
            else if (local_f0 == 0x42) {
              bVar8 = FUN_000e5878();
              FUN_000e57a0(bVar8 ^ 1);
            }
          }
        }
        uVar32 = thunk_FUN_0012a5b4();
        iVar14 = (int)((ulonglong)uVar32 >> 0x20);
        uVar15 = (uint)uVar32;
        if (0 < local_188) {
          DataMemoryBarrier(0xb);
          if (((*puVar22 & 1) == 0) && (iVar19 = FUN_002bdf44(puVar22), iVar19 != 0)) {
            *(undefined8 *)(iVar7 + 0x1b900) = uVar32;
            FUN_002be11c(puVar22);
          }
          puVar1 = (uint *)(DAT_0001bf10 + 0x1bd54);
          iVar19 = iVar14 - (*(int *)(DAT_0001bf10 + 0x1bd58) + (uint)(uVar15 < *puVar1));
          iVar23 = *(int *)(DAT_0001bf10 + 0x1bd5c) + 1;
          *(int *)(DAT_0001bf10 + 0x1bd5c) = iVar23;
          if (iVar19 != 0 && (uint)(uVar15 - *puVar1 < 0x2a05f200) <= 1U - iVar19) {
            dVar30 = (double)(longlong)iVar23 * DAT_0001bf00;
            dVar31 = (double)FUN_003807ec();
            dVar30 = dVar30 / dVar31;
            FUN_003807ec((int)lVar5,local_2f4);
            FUN_003807ec(local_2fc,local_2f8);
            FUN_003807ec(local_304,local_300);
            FUN_00032640(DAT_0001bf14 + 0x1bdec,extraout_r1_03,SUB84(dVar30,0),
                         (int)((ulonglong)dVar30 >> 0x20));
            iVar19 = DAT_0001bf18;
            *(undefined8 *)(DAT_0001bf18 + 0x1be08) = uVar32;
            *(undefined4 *)(iVar19 + 0x1be10) = 0;
            lVar5 = 0;
            lVar34 = 0;
            lVar4 = 0;
          }
        }
        local_2f8 = (int)((ulonglong)lVar34 >> 0x20);
        local_2fc = (uint)lVar34;
        local_300 = (int)((ulonglong)lVar4 >> 0x20);
        local_304 = (uint **)lVar4;
        local_2f4 = (int)((ulonglong)lVar5 >> 0x20);
        FUN_000e6ab8(&local_2c8);
        if (local_278 == 0) {
          dVar30 = (double)FUN_003807ec(uVar15 - uVar26,
                                        iVar14 - ((int)uVar6 + (uint)(uVar15 < uVar26)));
          dVar30 = dVar30 / DAT_0001bf00;
          uVar33 = thunk_FUN_0012a5b4();
          FUN_000e1260(iVar13,(float)dVar30);
          lVar34 = thunk_FUN_0012a5b4();
          FUN_00106b70(&local_2c4,&local_2c0);
          FUN_00105e68(local_2c4,local_2c0);
          FUN_000dc9ac(iVar13);
          FUN_0010f44c();
          lVar35 = thunk_FUN_0012a5b4();
          FUN_00106ba8();
          lVar36 = thunk_FUN_0012a5b4();
          if (*(char *)(iVar13 + 0xa70) != '\0') break;
          lVar4 = lVar34 + CONCAT44(local_300 -
                                    ((int)((ulonglong)uVar33 >> 0x20) +
                                    (uint)(local_304 < (uint)uVar33)),(int)local_304 - (uint)uVar33)
          ;
          lVar34 = lVar35 + CONCAT44(local_2f8 -
                                     ((int)((ulonglong)lVar34 >> 0x20) +
                                     (uint)(local_2fc < (uint)lVar34)),local_2fc - (uint)lVar34);
          lVar5 = lVar36 + CONCAT44(local_2f4 -
                                    ((int)((ulonglong)lVar35 >> 0x20) +
                                    (uint)((uint)lVar5 < (uint)lVar35)),(uint)lVar5 - (uint)lVar35);
        }
        else {
          iVar14 = FUN_0007b5f4(local_308);
          if (iVar14 == 0) {
            thunk_FUN_0012f4c4(0xf);
          }
          if (((uint)local_280[0] & 0xff) != 0) break;
        }
      } while (iVar27 == 0);
      FUN_0002e530();
      FUN_00022864(iVar13 + 0x15c);
      FUN_00105a9c();
      FUN_000e53a4();
      FUN_0002eb48();
      FUN_00106b00();
      thunk_FUN_00128698();
      uVar24 = 0;
      FUN_000e6fec(local_308);
    }
    FUN_000210c8(&local_298);
    FUN_000210c8(&local_2b0);
  }
  FUN_000e6fb8(&local_1d8);
  return uVar24;
}



