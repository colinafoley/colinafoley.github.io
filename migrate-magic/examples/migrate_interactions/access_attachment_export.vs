Option Compare Database

Public Function SaveAttachmentsTest() As Long
  Dim dbs As DAO.Database
  Dim rst As DAO.Recordset
  Dim rsA As DAO.Recordset2
  Dim rsB As String
  Dim fld As DAO.Field2
  Dim EntID As DAO.Field2
  Dim strFullPath As String
  Dim strPath As String
  Dim strPattern As String
  Dim tableName As String

  tableName = "tbl_entity"
  strPattern = "*.*"
  strPath = "C:\whatever"

  'Get the database, recordset, and attachment field
  Set dbs = CurrentDb
  Set rst = dbs.OpenRecordset(tableName)
  Set fld = rst("Ent_docs")
  Set EntID = rst("Ent_ID")

  'Navigate through the table
  Do While Not rst.EOF

    'Get the recordset for the Attachments field
    Set rsA = fld.Value
    rsB = EntID.Value

    'Save all attachments in the field
    Do While Not rsA.EOF
      If rsA("FileName") Like strPattern Then
        'To Export the data, use the line below
        strFullPath = strPath & "\" & tableName & "\" & EntID & "\" & rsA("FileName")

        'Make sure the file does not exist and save
        If Dir(strFullPath) = "" Then
          rsA("FileData").SaveToFile strFullPath
        End If

        'Increment the number of files saved
        SaveAttachmentsTest = SaveAttachmentsTest + 1
      End If
      'Next attachment
      rsA.MoveNext
    Loop
    rsA.Close
    'Next record
    rst.MoveNext
  Loop

  rst.Close
  dbs.Close
  Set fld = Nothing
  Set rsA = Nothing
  Set rst = Nothing
  Set dbs = Nothing
End Function
