::del .\RT1064\MDK\*.opt /s  ::不允许删除JLINK的设置

del .\RT1064\MDK\*.scvd /s
del .\RT1064\MDK\*.bak /s
del .\RT1064\MDK\*.ddk /s
del .\RT1064\MDK\*.edk /s
del .\RT1064\MDK\*.lst /s
del .\RT1064\MDK\*.lnp /s
del .\RT1064\MDK\*.mpf /s
del .\RT1064\MDK\*.mpj /s
del .\RT1064\MDK\*.obj /s
del .\RT1064\MDK\*.omf /s
del .\RT1064\MDK\*.plg /s
del .\RT1064\MDK\*.rpt /s
del .\RT1064\MDK\*.uvgui.* /s
del .\RT1064\MDK\*.tmp /s
del .\RT1064\MDK\*.__i /s
del .\RT1064\MDK\*.crf /s
del .\RT1064\MDK\*.o /s
del .\RT1064\MDK\*.d /s
del .\RT1064\MDK\*.axf /s
del .\RT1064\MDK\*.uvgui /s
del .\RT1064\MDK\*.uvgui* /s
del .\RT1064\MDK\*.bak /s
del .\RT1064\MDK\*.tra /s
del .\RT1064\MDK\*.dep /s
del .\RT1064\MDK\JLinkLog.txt /s

del .\RT1064\MDK\*.iex /s
del .\RT1064\MDK\*.htm /s
del .\RT1064\MDK\*.sct /s
del .\RT1064\MDK\*.map /s

rmdir .\RT1064\MDK\Objects /s /q
rmdir .\RT1064\MDK\Listings /s /q


rmdir .\RT1064\IAR\program\nor_sdram_zf_dtcm /s /q
rmdir .\RT1064\IAR\settings /s /q

del .\RT1064\IAR\*.crun /s
del .\RT1064\IAR\*.dbgdt /s
del .\RT1064\IAR\*.ps1 /s
del .\RT1064\IAR\*.ps1 /s
del .\RT1064\IAR\*.xcl /s
del .\RT1064\IAR\*.reggroups /s

pause
exit
