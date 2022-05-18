procedure eventMidi3; // Inline;
begin
{$IFDEF USE_FIFO}
  FIFO_Byte:=_event; FIFO_WriteByte;
  readB2FB; FIFO_WriteByte;
  readB2FB; FIFO_WriteByte;
{$ELSE}
  MC_Byte:=_event; MC6850_Send2;
  readB2FB; MC6850_Send2;
  readB2FB; MC6850_Send2;
{$ENDIF}
end;

procedure eventMidi2;
begin
{$IFDEF USE_FIFO}
  FIFO_Byte:=_event; FIFO_WriteByte;
  readB2FB; FIFO_WriteByte;
{$ELSE}
  MC_Byte:=_event; MC6850_Send2;
  readB2FB; MC6850_Send2;
{$ENDIF}
end;

procedure eventSysEx; // Inline;
begin
  readVarL;
{$IFDEF USE_FIFO}
  FIFO_Byte:=_event; FIFO_WriteByte;
{$ELSE}
  MC_Byte:=_event; MC6850_Send2;
{$ENDIF}
  while _tmp>0 do
  begin
    readB2FB;
{$IFDEF USE_FIFO}
    FIFO_WriteByte;
{$ELSE}
    MC6850_Send2;
{$ENDIF}
    dec(_tmp);
  end;
// {$IFDEF USE_FIFO}
//         if FIFO_Byte=$F7 then flagSysEx:=false else flagSysEx:=true;
// {$ELSE}
//         if MC_Byte=$F7 then flagSysEx:=false else flagSysEx:=true;
// {$ENDIF}

end;

procedure eventMeta; // Inline;
begin
  readB2FB; // fetch Meta event numer
  readVarL; // fetch data size
  // case FIFO_Byte of
  // case MC_Byte of
  // {$ENDIF}

  if {$IFDEF USE_FIFO}FIFO_Byte{$ELSE}MC_Byte{$ENDIF}=$2f then // end of track

    _status:=_status or f_stop

  else if {$IFDEF USE_FIFO}FIFO_Byte{$ELSE}MC_Byte{$ENDIF}=$51 then // set tempo
  begin
    read24;
    if _delta<>ms_per_qnote then
    begin
      ms_per_qnote:=_delta;
      setTempo;
    end;
  end
  {$IFDEF USE_SUPPORT_VARS}
  else if {$IFDEF USE_FIFO}FIFO_Byte{$ELSE}MC_Byte{$ENDIF}=$58 then // set tempo
  begin
    readB; tactNum:=_tmp;
    readB; tactDenum:=_tmp;
    readB; ticks_per_qnote:=_tmp;
    readB; ticks_per_32nd:=_tmp;
  end;
  {$ENDIF}
  else
  // any orther meta event are skipped
    while _tmp>0 do
    begin
      dec(_tmp);
      inc(_adr);
      memBoundCheck;
    end;
end;