#include "DataFrameModel.hpp"
#include "convert.hpp"

QVariant DataFrameModel::data(const QModelIndex &index, int role) const
{
  int col = index.column();
  int row = index.row();
  QVariant value;
  QModelIndex dummy;

  if (!index.isValid()) {
    qCritical("Model index is invalid");
    return value;
  }
  if (col >= columnCount(dummy)) {
    qCritical("Column index %d out of bounds", col);
    return value;
  }
  if (row >= rowCount(dummy)) {
    qCritical("Row index %d out of bounds", row);
    return value;
  }
  if (role >= length(_roles)) {
    qCritical("Role index %d out of bounds", role);
    return value;
  }

  SEXP roleVector = VECTOR_ELT(_roles, role);
  int dfIndex;
  if (roleVector == R_NilValue || (dfIndex = INTEGER(roleVector)[col]) == -1) {
    if (role == Qt::ToolTipRole)
      value = data(index, Qt::DisplayRole);
  } else value = qvariant_from_sexp(VECTOR_ELT(_dataframe, dfIndex), row);

  return value;
}

QVariant DataFrameModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
  QVariant value;
  QModelIndex dummy;
  if (section < 0) {
    qCritical("Invalid section number: %d", section);
    return value;
  }
  if (orientation == Qt::Horizontal) {
    if (section >= columnCount(dummy)) {
      qCritical("Column header index %d out of bounds", section);
      return value;
    }
    if (role >= length(_colHeader)) {
      qCritical("Column header role %d out of bounds", role);
      return value;
    }
    SEXP roleVector = VECTOR_ELT(_colHeader, role);
    if (roleVector != R_NilValue)
      value = qvariant_from_sexp(roleVector, section);
  } else {
    if (section >= rowCount(dummy)) {
      qCritical("Row header index %d out of bounds", section);
      return value;
    }
    if (role >= length(_rowHeader)) {
      qCritical("Row header role %d out of bounds", role);
      return value;
    }
    SEXP roleVector = VECTOR_ELT(_rowHeader, role);
    if (roleVector != R_NilValue)
      value = qvariant_from_sexp(roleVector, section);
  }
  return value;
}

void DataFrameModel::beginChanges(int nr, int nc) {
  if (_dataframe != R_NilValue) {
    int oldnr = rowCount(QModelIndex());
    int oldnc = columnCount(QModelIndex());
    if (oldnr > nr)
      beginRemoveRows(QModelIndex(), oldnr - 1L, nr);
    else if (oldnr < nr)
      beginInsertRows(QModelIndex(), oldnr, nr - 1L);
    if (oldnc > nc)
      beginRemoveColumns(QModelIndex(), oldnc - 1L, nc);
    else if (oldnc < nc)
      beginInsertColumns(QModelIndex(), oldnc, nc - 1L);
  }
}

void DataFrameModel::endChanges(int oldnr, int oldnc) {
  if (oldnr != -1) {
    int nr = rowCount(QModelIndex());
    int nc = columnCount(QModelIndex());
    if (oldnc > nc)
      endRemoveColumns();
    else if (oldnc < nc)
      endInsertColumns(); // just in case column names/roles changed
    else headerDataChanged(Qt::Horizontal, 0, nc);
    if (oldnr > nr) // insert rows
      endRemoveRows();
    else if (oldnr < nr)
      endInsertRows();
    else headerDataChanged(Qt::Vertical, 0, nr);
    // be lazy and just say everything changed
    // will not matter unless many rows/cols are in view (rare)
    dataChanged(index(0, nr), index(0, nc));
  }
}

void DataFrameModel::setDataFrame(SEXP dataframe, SEXP roles, SEXP rowHeader,
                                  SEXP colHeader)
{
  R_PreserveObject(dataframe);
  R_PreserveObject(roles);
  R_PreserveObject(rowHeader);
  R_PreserveObject(colHeader);

  // need dimension changes up-front
  beginChanges(headerLength(rowHeader), headerLength(colHeader));

  int oldnr = rowCount(QModelIndex()); // returns -1 if no dataframe
  int oldnc = columnCount(QModelIndex());
  
  _dataframe = dataframe;
  _roles = roles;
  _rowHeader = rowHeader;
  _colHeader = colHeader;

  // finish change notifications
  endChanges(oldnr, oldnc);
}

DataFrameModel::~DataFrameModel() {
  R_ReleaseObject(_dataframe);
  R_ReleaseObject(_roles);
  R_ReleaseObject(_rowHeader);
  R_ReleaseObject(_colHeader);
}

extern "C"
SEXP qt_qdataFrameModel() {
  SEXP dfm = wrapSmoke(new DataFrameModel, QAbstractTableModel, true);
  SEXP smokeClass, rClass;
  /* tricky: prepend class, which is not known to Smoke */
  smokeClass = getAttrib(dfm, R_ClassSymbol);
  PROTECT(rClass = allocVector(STRSXP, length(smokeClass) + 1));
  for (int i = 0; i < length(smokeClass); i++)
    SET_STRING_ELT(rClass, i+1, STRING_ELT(smokeClass, i));
  SET_STRING_ELT(rClass, 0, mkChar("DataFrameModel"));
  setAttrib(dfm, R_ClassSymbol, rClass);
  UNPROTECT(1);
  return dfm;
}

extern "C"
SEXP qt_qsetDataFrame(SEXP rmodel, SEXP df, SEXP roles, SEXP rowHeader,
                      SEXP colHeader)
{
  DataFrameModel *model =
    static_cast<DataFrameModel *>(unwrapSmoke(rmodel, QAbstractTableModel));
  model->setDataFrame(df, roles, rowHeader, colHeader);
  return R_NilValue;
}

extern "C"
SEXP qt_qdataFrame(SEXP rmodel) {
  DataFrameModel *model =
    static_cast<DataFrameModel *>(unwrapSmoke(rmodel, QAbstractTableModel));
  return model->dataFrame();
}
