import styles from './UserModal.module.css';

export default function UserModal({ isOpen, onClose, user }) {
  if (!isOpen) {
    return null;
  }

  return (
    <div className={styles.backdrop} onClick={onClose} role="presentation">
      <div className={styles.modal} onClick={(e) => e.stopPropagation()} role="dialog" aria-modal="true">
        <div className={styles.header}>
          <h2 className={styles.title}>Account Details</h2>
          <button type="button" className={styles.closeBtn} onClick={onClose} aria-label="Close user details">
            ×
          </button>
        </div>

        <div className={styles.body}>
          <div className={styles.row}>
            <span className={styles.label}>User ID</span>
            <span className={styles.value}>{user?.id ?? 'Unknown'}</span>
          </div>
          <div className={styles.row}>
            <span className={styles.label}>Username</span>
            <span className={styles.value}>{user?.username ?? 'Unknown'}</span>
          </div>
          <div className={styles.row}>
            <span className={styles.label}>Role</span>
            <span className={styles.value}>{user?.role ?? 'Unknown'}</span>
          </div>
        </div>
      </div>
    </div>
  );
}